
from rd_parser import *
from lexer import *
from instructions import *
from runtime_types import StaticType
import struct

class CompileError(Exception):
    def __init__(self,message,source):
        self.message = message;
        self.source = source

def warn(str):
    print("Compiler warning: ",str)

 

class StaticTable:
    def __init__(self):
        self.statics = {}

    def get_static_id(self,type,value):
        key = (type,value)
        if key not in self.statics:
            self.statics[key] = len(self.statics)
        return self.statics[key]       
    def get_string_id(self,value):
        return self.get_static_id(StaticType.STRING, value)        
    def get_name_id(self,name):
        return self.get_static_id(StaticType.NAME, name)
    def get_floating_id(self,floating):
        return self.get_static_id(StaticType.FLOATING, floating)
    def get_integer32_id(self,integer):
        return self.get_static_id(StaticType.INTEGER32, integer)
    def get_integer64_id(self,integer):
        return self.get_static_id(StaticType.INTEGER64, integer)
    def get_macro_id(self,integer):
        return self.get_static_id(StaticType.MACRO, integer)
        
    
    def to_binary(self):
        keys = list(self.statics.keys())
        keys.sort(key=lambda x:self.statics[x])
        binary = b""
        for type,value in keys:
            if type == StaticType.NAME:
                s = value.encode("utf-8")
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
            elif type == StaticType.STRING:
                s = value.encode("utf-8")
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
            elif type == StaticType.FLOATING:
                s = str(value).encode("utf-8")
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
            elif type == StaticType.INTEGER64:
                binary += struct.pack("=Bq",type,value)
            elif type == StaticType.INTEGER32:
                binary += struct.pack("=Bi",type,value)
            elif type == StaticType.MACRO:
                s = value.encode("utf-8")
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
        return binary
    def dump(self):
        keys = list(self.statics.keys())
        keys.sort(key=lambda x:self.statics[x])
        for type,value in keys:
            print(self.statics[(type,value)],value,type)
        
        
    def length(self):
        return len(self.statics)




class Compiler:
    def __init__(self):
        self.static_table = StaticTable()  
        self.global_scope = None
        self.local_scopes = []

    def get_identifier(self,name):
        if len(self.local_scopes) > 0:
            name_obj = self.local_scopes[-1].get_name(name)
        else:
            name_obj = self.global_scope.get_name(name)
        static_id = self.static_table.get_name_id(name)
        return Identifier(name_obj.global_id,name_obj.local_id,static_id)

    
    def set_global_scope(self,scope):
        self.global_scope = scope
    def push_local_scope(self,scope):
        self.local_scopes.append(scope)
    def pop_local_scope(self):
        self.local_scopes.pop()
    def resolve_addresses(self,code):


        def resolve_address(address):
            if address.type == Address.UNRESOLVED_ABSOLUTE:
                return address.resolve(index)
            else:
                return address

        for index,instruction in enumerate(code):
            address = None
            if hasattr(instruction,"address"):
                instruction.address = resolve_address(instruction.address)
            if hasattr(instruction,"continue_address"):
                instruction.continue_address = resolve_address(instruction.continue_address)
            if hasattr(instruction,"exit_address"):
                instruction.exit_address = resolve_address(instruction.exit_address)


    def resolve_loop_jump_address(self,code,start_pos,end_pos):
        for index,instruction in enumerate(code):
            if not hasattr(instruction,"address"): continue
            address = instruction.address
            if address.type == Address.UNRESOLVED_LOOP_JUMP:
                if address.pop_level() == 0:
                    instruction.address = address.resolve(index,start_pos,end_pos)

    def compile_function(self,function):

        self.push_local_scope(function.scope)
        compiled_body = []
        arguments = function.nodes[Function.NODE_ARGUMENTS].nodes[ArgumentList.NODE_ARGUMENT_LIST]
        for argument in arguments:
            is_byref = Argument.NODE_BYREF in argument.nodes
            if is_byref:
                compiled_body += [CreateByRefArgumentInstruction(self.get_identifier(argument.nodes[Argument.NODE_NAME].value))]
            else:
                compiled_body += [CreateArgumentInstruction(self.get_identifier(argument.nodes[Argument.NODE_NAME].value))]

        
        for name in function.scope.names.values():
            if name.closed:
                compiled_body += [CreateClosureNameInstruction(self.get_identifier(name.name))]

        default_value_found = False
        without_default = 0
        compiled_arguments_init = []
        for argument in arguments:
            ident = self.get_identifier(argument.nodes[Argument.NODE_NAME].value)
            if Argument.NODE_CONST in argument.nodes:
                
                compiled_arguments_init += [MakeLocalConstInstruction(ident)]
            if Argument.NODE_DEFAULT_VALUE in argument.nodes:
                compiled_arguments_init += [PushNameValueInstruction(ident),PushDefaultInstruction(),ExactlyEqualInstruction()]
                compiled_default_value = self.compile_expression(argument.nodes[Argument.NODE_DEFAULT_VALUE])
                compiled_default_value += [AssignLocalInstruction(ident)]
                compiled_arguments_init += [JumpIfFalseInstruction(RelativeAddress(len(compiled_default_value)+1))] + compiled_default_value
                default_value_found = True
            else:
                without_default += 1
                if default_value_found:
                    raise CompileError("Function definition error!")

        compiled_body += [LoadArgumentsInstruction(len(arguments),without_default)]   + compiled_arguments_init        

        if Function.NODE_BODY in function.nodes:
            compiled_body += self.compile_block(function.nodes[Function.NODE_BODY])
        
        compiled_body += [PushNullInstruction(),RetInstruction()]
        
        self.pop_local_scope()

        function_offset = 2
        if Function.NODE_NAME in function.nodes:
            function_offset += 1
        
        code = [PushFunctionInstruction(UnresolvedAbsoluteAddress(function_offset))]

        if Function.NODE_NAME in function.nodes:
            code += [AssignNearestInstruction(self.get_identifier(function.nodes[Function.NODE_NAME].value))]

        code += [JumpInstruction(RelativeAddress(len(compiled_body)+1))]
        code += compiled_body
        
        

        return code
    
    def compile_declaration(self,declaration):
        code = []
        
        scope_token = declaration.nodes[Declaration.NODE_SCOPE]

        is_const = Declaration.NODE_CONST in declaration.nodes
        const_instruction = None
        if scope_token.value == KeywordToken.DIM:
            assignment_instruction = AssignNearestInstruction
            if is_const:
                const_instruction = MakeNearestConstInstruction
        elif scope_token.value == KeywordToken.GLOBAL:
            assignment_instruction = AssignGlobalInstruction
            if is_const:
                const_instruction = MakeGlobalConstInstruction
        elif scope_token.value == KeywordToken.LOCAL:
            assignment_instruction = AssignLocalInstruction
            if is_const:
                const_instruction = MakeLocalConstInstruction

        if Declaration.NODE_ENUM in declaration.nodes:
            code += self.compile_enum(declaration.nodes[Declaration.NODE_ENUM],assignment_instruction,const_instruction)
            return code



        for variable in declaration.nodes[Declaration.NODE_VARIABLES]:
            # If the variable has a right hand side, compile it with and use it as assignment.
            if DeclarationAssignment.NODE_VALUE_EXPRESSION in variable.nodes:
                code += self.compile_expression(variable.nodes[DeclarationAssignment.NODE_VALUE_EXPRESSION])
                if len(DeclarationAssignment.NODE_SUBSCRIPTS)>0 in variable.nodes:
                    warn("Left hand array declaration ignored in favor for right hand expression.")
            # Does the declaration declare an array such as: Dim a[10][2], compile it with
            # special CreateMultiDimList instruction.
            elif DeclarationAssignment.NODE_SUBSCRIPTS in variable.nodes:
                for list_index in variable.nodes[DeclarationAssignment.NODE_SUBSCRIPTS]:
                    code += self.compile_expression(list_index.nodes[ListIndexing.NODE_INDEX])
                code += [CreateMultiDimListInstruction(len(variable.nodes[DeclarationAssignment.NODE_SUBSCRIPTS]))]
            # Empty variable are assigned to Null.
            else:
                code += [PushNullInstruction()]
            ident = self.get_identifier(variable.nodes[DeclarationAssignment.NODE_IDENTIFIER].value)
            code += [assignment_instruction(ident)]
            if is_const:
                code += [const_instruction(ident)]

        return code
    
    def compile_return(self,statement):
        code = []
        
        # Return expression or NULL.
        if Return.NODE_EXPRESSION in statement.nodes:
            code += self.compile_expression(statement.nodes[Return.NODE_EXPRESSION])
        else:
            code += [PushNullInstruction()]
        
        code += [RetInstruction()]
        return code
    
    def compile_if(self,statement):
        
        compiled_condition = self.compile_expression(statement.nodes[If.NODE_CONDITION])
        
        # Inline if-statements are easy to compile!
        if If.NODE_INLINE_STATEMENT in statement.nodes:
            compiled_inline_statement = self.compile_statement(statement.nodes[If.NODE_INLINE_STATEMENT])
            return compiled_condition + [JumpIfFalseInstruction(RelativeAddress(len(compiled_inline_statement)+1))] + compiled_inline_statement
        

        # Full blown if statements with possible elseifs and else is
        # harder. There are a lot of jump offsets that needs to be computed.

        # Contains this structure:
        # If         (condition,body) 
        # ElseIf     (condition,body)
        # ElseIf     (condition,body)
        # Else       (,body)
        #
        compiled_components = []
        
        compiled_body = []
        if If.NODE_BODY in statement.nodes:
            compiled_body = self.compile_block(statement.nodes[If.NODE_BODY])
        else:
            compiled_body = []
        compiled_components.append((compiled_condition+[JumpIfFalseInstruction(None)],compiled_body+[JumpInstruction(None)]))
        
        if If.NODE_ELSEIFS in statement.nodes:
            for elseif in statement.nodes[If.NODE_ELSEIFS]:
                elseif_compiled_condition = self.compile_expression(elseif.nodes[ElseIf.NODE_CONDITION])
                # Add dummy jump instruction to NEXT elseif/else/endif
                elseif_compiled_condition += [JumpIfFalseInstruction(None)]

                elseif_compiled_body = []
                if ElseIf.NODE_BODY in elseif.nodes:
                    elseif_compiled_body = self.compile_block(elseif.nodes[ElseIf.NODE_BODY])
                # Add jump to endif
                elseif_compiled_body += [JumpInstruction(None)]

                compiled_components.append((elseif_compiled_condition,elseif_compiled_body))

        if If.NODE_ELSE in statement.nodes:
            compiled_body = []
            if Else.NODE_BODY in statement.nodes:
                compiled_body = self.compile_block(statement.nodes[If.NODE_ELSE].nodes[Else.NODE_BODY])
            compiled_body += [JumpInstruction(None)]
            compiled_components += [([],compiled_body)]

        # How many instructions does this whole statement contain?
        # Works by first mapping the component structure to a new list
        # containing each sum of each component which are then summed together.
        number_of_instructions = sum(map(lambda x:len(x[0])+len(x[1]),compiled_components))
        consumed_instructions = 0
        output_instructions = []

        # Resolve all the jump addresses. Complicated :(
        for index,component in enumerate(compiled_components):
            
            condition, body = component

            # Calculate condition jump offset.
            if condition: # If the component has a condition (only else lacks this).
                consumed_instructions += len(condition)-1
                condition[-1].address = RelativeAddress(len(body)+1)
                consumed_instructions += 1

            consumed_instructions += len(body)-1
            body[-1].address = RelativeAddress(number_of_instructions - consumed_instructions)
            consumed_instructions += 1

            output_instructions += condition + body
        
        
    
        return output_instructions

    def compile_dountil(self,statement):
        compiled_body = self.compile_block(statement.nodes[DoUntil.NODE_BODY])
        compiled_condition = self.compile_expression(statement.nodes[DoUntil.NODE_CONDITION])
        code = [PushLoopBlockInstruction(None,None)]
        code += compiled_body
        code += compiled_condition
        code += [JumpIfFalseInstruction(RelativeAddress(-len(code)+1))]
        code +=[PopBlockInstruction()]

        code[0].continue_address = UnresolvedAbsoluteAddress(len(code)-len(compiled_condition)-2)
        code[0].exit_address = UnresolvedAbsoluteAddress(len(code))

        return code

    # A select statement is very much alike an if-statement.
    # The statement contains a list of expressions where the first
    # to evaluate to true is executed and an optional else clause
    # is executed if no other expression evaluates to true.
    def compile_select(self,select):
        code = []

        # Loop through all the case blocks.
        for index,case in enumerate(select.nodes[Select.NODE_CASE_LIST]):
            # Compile the body of the case first, as it doesn't depend on the conditions
            # but the conditions depends on the length of this block.
            compiled_body = self.compile_block(case.nodes[SelectCase.NODE_BODY])
            # After body is complete, jump to the end of the while select statement.
            compiled_body += [JumpInstruction("END")]

            # Is this case statement a "case else" statement?
            if SelectCase.NODE_ELSE in case.nodes:
                # Only the last one is valid!
                if index != len(select.nodes[Select.NODE_CASE_LIST])-1:
                    raise CompileError("else must be the last case statement in select statement",case.nodes[SelectCase.NODE_ELSE].source)
                # No need to compile anything here, just go straight to body.
            else:
                # Compile condition with matching jumpiffalse instruction.
                code += self.compile_expression(case.nodes[SelectCase.NODE_CONDITION])
                code += [JumpIfFalseInstruction(RelativeAddress(len(compiled_body)+1))]           
            code += compiled_body

        # There are now instructions that point to symbolic address "END".
        # Fill them in.
        for pos,instruction in enumerate(code):
            if hasattr(instruction,"address") and instruction.address == "END":
                instruction.address = RelativeAddress(len(code)-pos)

        return code

    # Switch statements are very complex and powerful!
    # They are therefore probably the hardest to compile
    # and they generate a lot of code.
    def compile_switch(self,switch):
        # Switch statements keeps the value being switched on top of
        # the stack during the execution.
        # We therefore need to push a block so stack gets adjusted if for example
        # a continueloop is executed.
        code = [PushGeneralBlockInstruction()]

        # Compile expression being switched on.
        code += self.compile_expression(switch.nodes[Switch.NODE_VALUE])

        for index,case in enumerate(switch.nodes[Switch.NODE_CASES]):

            compiled_body = self.compile_block(case.nodes[SwitchCase.NODE_BODY])
            compiled_conditions = []

            # Every case can have multiple conditions *sigh*
            conditions = case.nodes[SwitchCase.NODE_CONDITIONS]

            # Loop through them all and compile them.
            for cond_index,condition in enumerate(conditions):
                if SwitchCondition.NODE_ELSE in condition.nodes:
                    if index != len(switch.nodes[Switch.NODE_CASES])-1:
                        raise CompileError("else must be the last case statement in switch statement",case.nodes[SwitchCondition.NODE_ELSE].source)
                elif SwitchCondition.NODE_TO in condition.nodes:
                    # Here we have a <expression> to <expression> syntax.
                    # This means that we have to do TWO compares on the value being switched.
                    # Create two copies of the value then.
                    code += [DoubleTopInstruction()]
                    code += [DoubleTopInstruction()]

                    # Compile the conditions.
                    compiled_first_condition = self.compile_expression(condition.nodes[SwitchCondition.NODE_FROM])
                    compiled_second_condition = self.compile_expression(condition.nodes[SwitchCondition.NODE_TO])

                    code += compiled_first_condition
                    code += [GreaterEqualInstruction()]
                    # We now have the result of the "from" expression on TOS.
                    # However we now need to have the value on top as we need to do another comparison.
                    # Since the value is just below the boolean on top, swap them!
                    code += [SwapTopInstruction()]
                    code += compiled_second_condition
                    code += [LesserEqualInstruction()]
                    # We now have the two results on TOS. Since they're both boolean they
                    # fit perfectly for this next and instruction.
                    code += [BooleanAndInstruction()]

                    # This is important.
                    # If this is the last condition in the case, we must jump
                    # to the next case statement if it's false, but if it's not
                    # we need to jump to the next condition.
                    if cond_index == (len(conditions)-1):
                        code += [JumpIfFalseInstruction(RelativeAddress(len(compiled_body)+2))]
                    else:
                        # Next instruction is jump the the beginning of the body.
                        # At the instruction after that is the next condition, jump there!
                        code += [JumpIfFalseInstruction(RelativeAddress(2))]
                        code += [JumpInstruction("BODY_START")]

                else:
                    # This is a simplier version of the above where we only have to "from" value
                    # and just need to do a single equal comparison.
                    compiled_first_condition = self.compile_expression(condition.nodes[SwitchCondition.NODE_FROM])
                    code += [DoubleTopInstruction()]
                    code += compiled_first_condition + [EqualInstruction()]
                    if cond_index == (len(conditions)-1):
                        code += [JumpIfFalseInstruction(RelativeAddress(len(compiled_body)+2))]
                    else:
                        code += [JumpIfFalseInstruction(RelativeAddress(2))]
                        code += [JumpInstruction("BODY_START")]
            # Resolve the "BODY_START" symbolic addresses.
            for pos,instruction in enumerate(code):
                if hasattr(instruction,"address") and instruction.address == "BODY_START":
                    instruction.address = RelativeAddress(len(code)-pos)

            code += compiled_body
            code += [JumpInstruction("END")]
        # Resolve END symbolic adresses-
        for pos,instruction in enumerate(code):
            if hasattr(instruction,"address") and instruction.address == "END":
                instruction.address = RelativeAddress(len(code)-pos)
        # Finally we pop the value being switched on (poor bastard) and
        # then pop the block. Puh. This statement is hard work!
        code += [PopInstruction(),PopBlockInstruction()]
        return code

    def compile_for(self,for_stmt):
        # The loop variable.
        loop_var = for_stmt.nodes[For.NODE_LOOP_VARIABLE]

        loop_var_id = self.get_identifier(loop_var.value)

        # Compile body.
        compiled_body = []
        if For.NODE_BODY in for_stmt.nodes:
            compiled_body = self.compile_block(for_stmt.nodes[For.NODE_BODY])

        # There are two types of for loops.
        # We only support for variable = init to end [step int] atm.
        if For.NODE_FOR_TO in for_stmt.nodes:
            forto = for_stmt.nodes[For.NODE_FOR_TO]
            # Compile init value and assign it. This code is only executed once!
            compiled_init = self.compile_expression(forto.nodes[ForTo.NODE_INIT_EXPRESSION])
            compiled_init += [AssignLocalInstruction(loop_var_id)]
            compiled_init += self.compile_expression(forto.nodes[ForTo.NODE_END_EXPRESSION])

            # Get the step value.
            # Due to the difference in comparison operator
            # for negative/positive values a constant int terminal
            # is required as step value.
            step_value = 1
            if ForTo.NODE_STEP_VALUE in forto.nodes:
                number_terminal = forto.nodes[ForTo.NODE_STEP_VALUE]
                if NumberTerminal.NODE_NEGATIVE in number_terminal.nodes:
                    step_value = -number_terminal.nodes[NumberTerminal.NODE_NUMBER].value
                else:
                    step_value = number_terminal.nodes[NumberTerminal.NODE_NUMBER].value

            # Compile check to see if loop should continue.
            compiled_check=[DoubleTopInstruction()]
            compiled_check += [PushNameValueInstruction(loop_var_id)]
            
            if step_value > 0:
                compiled_check += [GreaterEqualInstruction()]
            elif step_value <0:
                compiled_check += [LesserEqualInstruction()]
            else:
                raise CompileError("Invalid step value!")
            compiled_check += [JumpIfFalseInstruction(RelativeAddress(None))]

            # Compilation of code to increment (or decrement) loop variable.
            compiled_increment = [PushNameValueInstruction(loop_var_id)]
            compiled_increment += [PushInteger32Instruction(self.static_table.get_integer32_id(step_value))]
            compiled_increment += [AdditionInstruction()]
            compiled_increment += [AssignLocalInstruction(loop_var_id)]
            compiled_increment += [JumpInstruction(RelativeAddress(None))]

            # Calculate some addresses (sorry about this).
            compiled_check[-1].address.value = len(compiled_body)+len(compiled_increment)+1
            compiled_increment[-1].address.value = -(len(compiled_increment)+len(compiled_body)+len(compiled_check)-1)

            # Glue it all together!
            code = compiled_init + compiled_check + compiled_body + compiled_increment + [PopInstruction(),PopBlockInstruction()]
            code = [PushLoopBlockInstruction(UnresolvedAbsoluteAddress(len(code)-len(compiled_increment)-1),
                                             UnresolvedAbsoluteAddress(len(code)+1))] + code

            return code

    def compile_exit(self,exit_statement):
        code = []
        # Providing an expresion to exit for return value is optional
        # and results in Exit 0 if omitted.
        if Exit.NODE_EXPRESSION in exit_statement.nodes:
            code += self.compile_expression(exit_statement.nodes[Exit.NODE_EXPRESSION])
        else:
            code += [PushInteger32Instruction(self.static_table.get_integer32_id(0))]
        code += [TerminateInstruction()]
        return code

    def compile_continueloop(self,continue_statement):
        # The runtime takes care of the hard work. Pew!
        if ContinueLoop.NODE_LEVEL in continue_statement.nodes:
            level = continue_statement.nodes[ContinueLoop.NODE_LEVEL].value
        else:
            level = 1
        return [ContinueLoopInstruction(level)]

    
    def compile_exitloop(self,exitloop_statement):
        # The runtime takes care of the hard work. Pew!
        if ExitLoop.NODE_LEVEL in exitloop_statement.nodes:
            level = exitloop_statement.nodes[ExitLoop.NODE_LEVEL].value
        else:
            level = 1
        return [BreakLoopInstruction(level)]

    # This is a leftover from autoit3 where a array (list) could
    # be resized in multiple dimensions. 
    def compile_redim(self,redim):
        code = []
        code += [PushNameValueInstruction(self.get_identifier(redim.nodes[ReDim.NODE_NAME].value))]
        for qualifier in redim.nodes[ReDim.NODE_QUALIFIERS]:
            code += self.compile_expression(qualifier.nodes[ListIndexing.NODE_INDEX])
        code += [RedimMultiDimListInstruction(len(redim.nodes[ReDim.NODE_QUALIFIERS]))]
        return code

    def compile_enum(self,enum,assignment_instruction=AssignNearestInstruction,const_instruction=None):
        code = []
        value = 0
        if Enum.NODE_STEP in enum.nodes:
            
            step = enum.nodes[Enum.NODE_STEP]
            step_value = step.nodes[EnumStep.NODE_VALUE].value
            if EnumStep.NODE_OPERATOR in step.nodes:
                op = step.nodes[EnumStep.NODE_OPERATOR]
                if op.value == OperatorToken.ADD:
                    step_function = lambda x:x+step_value
                elif op.value == OperatorToken.SUBTRACT:
                    step_function = lambda x:x-step_value
                elif op.value == OperatorToken.MULTIPLY:
                    step_function = lambda x:x*step_value
                    value = 1
            else:
                step_function = lambda x:x+step_value
        else:
            step_function = lambda x:x+1

        for constant in enum.nodes[Enum.NODE_ENUM_LIST].nodes[EnumList.NODE_CONSTANTS]:
            if EnumConstant.NODE_VALUE in constant.nodes:
                value = constant.nodes[EnumConstant.NODE_VALUE].value
            code += [PushInteger32Instruction(self.static_table.get_integer32_id(value))]
            ident = self.get_identifier(constant.nodes[EnumConstant.NODE_IDENTIFIER].value)
            code += [assignment_instruction(ident)]
            if const_instruction:
                code += [const_instruction(ident)]
            value = step_function(value)
        return code
            

    def compile_statement(self,statement):
        substatement = statement.nodes[Statement.NODE_SUBSTATEMENT]
        if substatement.type == Rule.FUNCTION:
            return self.compile_function(substatement)
        if substatement.type == Rule.WHILE:
            return self.compile_while_statement(substatement)
        if substatement.type == Rule.LINE_STATEMENT:
            return self.compile_line_statement(substatement)
        if substatement.type == Rule.DECLARATION:
            return self.compile_declaration(substatement)
        if substatement.type == Rule.RETURN:
            return self.compile_return(substatement)
        if substatement.type == Rule.IF:
            return self.compile_if(substatement)
        if substatement.type == Rule.DO_UNTIL:
            return self.compile_dountil(substatement)
        if substatement.type == Rule.FOR:
            return self.compile_for(substatement)
        if substatement.type == Rule.EXIT:
            return self.compile_exit(substatement)
        if substatement.type == Rule.CONTINUELOOP:
            return self.compile_continueloop(substatement)
        if substatement.type == Rule.EXITLOOP:
            return self.compile_exitloop(substatement)
        if substatement.type == Rule.REDIM:
            return self.compile_redim(substatement)
        if substatement.type == Rule.ENUM:
            return self.compile_enum(substatement)
        if substatement.type == Rule.SELECT:
            return self.compile_select(substatement)
        if substatement.type == Rule.SWITCH:
            return self.compile_switch(substatement)
        warn("No compiler found for statement: "+substatement.type)
        return []
                
    def compile_list_indexing(self,indexing):
        code = []
        code += self.compile_expression(indexing.nodes[ListIndexing.NODE_INDEX])
        code += [IndexInstruction()]
        return code

    def compile_qualifier(self,qualifier):
        if qualifier.nodes[Qualifier.NODE_SUBQUALIFIER].type == Rule.CALL:
            return self.compile_call(qualifier.nodes[Qualifier.NODE_SUBQUALIFIER])
        if qualifier.nodes[Qualifier.NODE_SUBQUALIFIER].type == Rule.LIST_INDEXING:
            return self.compile_list_indexing(qualifier.nodes[Qualifier.NODE_SUBQUALIFIER])
        
    def compile_call(self,call):
        code = []
        for expression in call.nodes[Call.NODE_ARGUMENTS]:
            compiled_arg = self.compile_expression(expression)

            if len(compiled_arg) == 1 and isinstance(compiled_arg[0],PushNameValueInstruction):
                compiled_arg[0] = PushNameInstruction(compiled_arg[0].identifier)

            code += compiled_arg
        code += [CallFunctionInstruction(len(call.nodes[Call.NODE_ARGUMENTS]))]
        return code
    
    def compile_assignment(self,assignment,value_code,assignment_instruction):

        pre_expr_code = []
        expr_code = []
        post_expr_code = []

        assignment_op = assignment.nodes[Assignment.NODE_ASSIGNMENT_OPERATOR]

        if assignment_op.value == OperatorToken.ADD_ASSIGN:
            pre_expr_code += value_code
            post_expr_code += [AdditionInstruction()]
        if assignment_op.value == OperatorToken.SUBTRACT_ASSIGN:
            pre_expr_code += value_code
            post_expr_code += [SubtractionInstruction()]
        if assignment_op.value == OperatorToken.MULTIPLY_ASSIGN:
            pre_expr_code += value_code
            post_expr_code += [MultiplicationInstruction()]
        if assignment_op.value == OperatorToken.DIVIDE_ASSIGN:
            pre_expr_code += value_code
            post_expr_code += [DivisionInstruction()]
        if assignment_op.value == OperatorToken.CONCAT_ASSIGN:
            pre_expr_code += value_code
            post_expr_code += [ConcatInstruction()]
        # Compile expression
        expr_code += self.compile_expression(assignment.nodes[Assignment.NODE_VALUE_EXPRESSION])

        post_expr_code += [assignment_instruction]

        return pre_expr_code + expr_code + post_expr_code
    
    def compile_qualifiers(self,qualifiers):
        code = []
        while len(qualifiers) > 0:
            code += self.compile_qualifier(qualifiers.pop(0))
        return code
    
    def compile_line_statement(self,line_statement):
        nodes = line_statement.nodes
        code = []
        
        ident = nodes[LineStatement.NODE_START]
        code.append(PushNameValueInstruction(self.get_identifier(ident.value)))
        
        qualifiers = nodes[LineStatement.NODE_QUALIFIERS]
        code += self.compile_qualifiers(qualifiers)

        if LineStatement.NODE_ASSIGNMENT in nodes:
            assignment = nodes[LineStatement.NODE_ASSIGNMENT]

            # remove last instruction
            last_instruction = code.pop()
            
            if isinstance(last_instruction,PushNameValueInstruction):
                code += self.compile_assignment(assignment,[last_instruction],AssignNearestInstruction(last_instruction.identifier))  
            if isinstance(last_instruction,IndexInstruction):
                code += self.compile_assignment(assignment,[DoubleTopTwoInstruction(),last_instruction],AssignIndexInstruction())  

        else:
            code += [PopInstruction()]       
        
        return code
    
    def compile_while_statement(self,while_statement):

        
        compiled_condition = self.compile_expression(while_statement.nodes[While.NODE_CONDITION])
        compiled_body = []
        if While.NODE_BODY in while_statement.nodes:
            compiled_body = self.compile_block(while_statement.nodes[While.NODE_BODY])
        
        code = [PushLoopBlockInstruction(UnresolvedAbsoluteAddress(1),None)]
        code += compiled_condition
        code += [JumpIfFalseInstruction(RelativeAddress(len(compiled_body)+2))]
        code += compiled_body
        code += [JumpInstruction(RelativeAddress(-(len(compiled_body)+len(compiled_condition)+1)))]
        code += [PopBlockInstruction()]

        code[0].exit_address = UnresolvedAbsoluteAddress(len(code))

        self.resolve_loop_jump_address(code,0,len(code))

        return code
    
    def compile_operator(self,operator):
        token = operator.nodes[BinaryOperator.NODE_OPERATOR]
        
        return [{OperatorToken.ADD: AdditionInstruction,
                 OperatorToken.SUBTRACT: SubtractionInstruction,
                 OperatorToken.MULTIPLY: MultiplicationInstruction,
                 OperatorToken.DIVIDE: DivisionInstruction,
                 OperatorToken.POW: PowInstruction,
                 OperatorToken.GREATER: GreaterInstruction,
                 OperatorToken.GREATER_EQUAL: GreaterEqualInstruction,
                 OperatorToken.LESSER: LesserInstruction,
                 OperatorToken.LESSER_EQUAL: LesserEqualInstruction,
                 OperatorToken.EQUAL: EqualInstruction,
                 OperatorToken.BOOLEAN_AND: BooleanAndInstruction,
                 OperatorToken.BOOLEAN_OR: BooleanOrInstruction,
                 OperatorToken.CONCAT: ConcatInstruction,
                 OperatorToken.STRONG_STRING_EQUAL: StrongStringEqualInstruction,
                 OperatorToken.EXACTLY_EQUAL: ExactlyEqualInstruction,
                 OperatorToken.NOT_EQUAL: NotEqualInstruction
                 }[token.value]()]
        
        
        if token.value == OperatorToken.ADD:
            return [AdditionInstruction()]
        elif token.value == OperatorToken.MULTIPLY:
            return [MultiplicationInstruction()]
           
    def compile_unary_operator(self,unary):
        token = unary.nodes[UnaryOperator.NODE_OPERATOR]
        if token.value == OperatorToken.BOOLEAN_NOT:
            return [BooleanNotInstruction()]
        if token.value == OperatorToken.SUBTRACT:
            return [NegationInstruction()]

    def compile_terminal(self,terminal):
        term = terminal.nodes[Terminal.NODE_TYPE]
        if isinstance(term,Token):
            token = term
            if token.type == Token.INTEGER:
                if token.value >= -(2**31) and token.value < 2**31:
                    return [PushInteger32Instruction(self.static_table.get_integer32_id(token.value))]
                return [PushInteger64Instruction(self.static_table.get_integer64_id(token.value))]
            if token.type == Token.IDENTIFIER:
                return [PushNameValueInstruction(self.get_identifier(token.value))]
            if token.type == Token.STRING:
                return [PushStringInstruction(self.static_table.get_string_id(token.value))]   
            if token.type == Token.FLOATING:
                return [PushFloatingInstruction(self.static_table.get_floating_id(token.value))]     
            if token.type == Token.BOOLEAN:
                return [PushBooleanInstruction(token.value)]
            if token.type == Token.MACRO:
                return [PushMacroInstruction(self.static_table.get_macro_id(token.value))]
            if token.type == Token.KEYWORD and token.value == KeywordToken.DEFAULT:
                return [PushDefaultInstruction()]
        if isinstance(term,Rule):
            rule = term
            if rule.type == Rule.ANONYMOUS_FUNCTION:
                return self.compile_function(rule)
        

    def compile_inline_list(self,inline_list):
        code = []
        elements = inline_list.nodes[InlineList.NODE_ELEMENTS]
        for element in elements:
            code += self.compile_expression(element)
        code += [BuildListInstruction(len(elements))]
        return code

    def compile_inline_map(self,inline_map):
        key_values = inline_map.nodes[InlineMap.NODE_KEY_VALUES]
        code = []
        for key_value in key_values:
            key = key_value.nodes[KeyValue.NODE_KEY]
            value  = key_value.nodes[KeyValue.NODE_VALUE]
            code += self.compile_expression(key)
            code += self.compile_expression(value)
        code += [BuildMapInstruction(len(key_values))]
        return code

        
    def compile_factor(self,factor):
        rule = factor.nodes[Factor.NODE_SUBNODE]
        code = []
        
        if rule.type == Rule.TERMINAL:
            code = self.compile_terminal(rule)
        elif rule.type == Rule.EXPRESSION:
            code = self.compile_expression(rule)
        elif rule.type == Rule.INLINE_LIST:
            code = self.compile_inline_list(rule)
        elif rule.type == Rule.INLINE_MAP:
            code = self.compile_inline_map(rule)

            
        code += self.compile_qualifiers(factor.nodes[Factor.NODE_QUALIFIERS])

        if Factor.NODE_UNARIES in factor.nodes:
            for unary in reversed(factor.nodes[Factor.NODE_UNARIES]):
                code += self.compile_unary_operator(unary)
        
        return code
    
    def compile_expression(self,expr):
        nodes = expr.nodes
        code = self.compile_factor(nodes.pop(0))
        
        while nodes:
            op = nodes.pop(0)
            right_hand = nodes.pop(0)
            if right_hand.type == Rule.FACTOR:
                code += self.compile_factor(right_hand)
            elif right_hand.type == Rule.EXPRESSION:
                code += self.compile_expression(right_hand)
            code += self.compile_operator(op)
        return code    
    
    def compile_block(self,block):
        code = []
        for stm in block.nodes[Block.NODE_STATEMENTS]:
            code += self.compile_statement(stm)
        return code
    
    def compile_program(self,program):
        self.set_global_scope(program.scope)
        code = self.compile_block(program.nodes[Program.NODE_BLOCK]) + [PushInteger32Instruction(self.static_table.get_integer32_id(0)),TerminateInstruction()]
        self.resolve_addresses(code)
        return code
    


