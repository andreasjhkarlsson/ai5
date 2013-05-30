
from rd_parser import *
from lexer import *
from runtime_types import StaticType
from runtime_types import InstructionType
import struct


def fits_in_char(integer):
    return integer >= -128 and integer <= 127

class Identifier:
    def __init__(self,global_id,local_id,static_id):
        self.global_id = global_id
        self.local_id = local_id
        self.static_id = static_id
    def to_binary(self):
        return struct.pack("=III",self.global_id,self.local_id,self.static_id)
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
class Address:
    RELATIVE = "relative"
    ABSOLUTE = "absolute"
    UNRESOLVED_ABSOLUTE = "unresolved absolute"
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)

class RelativeAddress:
    type = Address.RELATIVE
    def __init__(self,offset):
        self.value = offset
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
class AbsoluteAddress:
    type = Address.ABSOLUTE
    def __init__(self,position):
        self.value = position
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)

# Can be used as a placeholder when absolute address is needed but absolute address
# needs to be calculated later.
class UnresolvedAbsoluteAddress:
    type = Address.UNRESOLVED_ABSOLUTE
    def __init__(self,offset):
        self.offset = offset
    def resolve(self,position):
        return AbsoluteAddress(position + self.offset)



class Instruction:
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
    def to_binary_without_arg(self,type):
        return struct.pack("=B",type)
    def to_binary_with_char_arg(self,type,char):
        return struct.pack("=Bb",type,char)
    def to_binary_with_int_arg(self,type,int):
        return struct.pack("=Bi",type,int)
    def to_binary_with_int64_arg(self,type,int64):
        return struct.pack("=Bq",type,int64)
    def to_binary_with_double_arg(self,type,double):
        return struct.pack("=Bd",type,double)

class IndexInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.INDEX)

class PushInteger32Instruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_INTEGER32, self.id)

class PushInteger64Instruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_INTEGER64, self.id)

class AssignGlobalConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_GLOBAL_CONST) + self.identifier.to_binary()

class AssignLocalConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_LOCAL_CONST) + self.identifier.to_binary()
class AssignNearestConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_NEAREST_CONST) + self.identifier.to_binary()
class PushFunctionInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_FUNCTION, self.address.value)    


class PushStringInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_STRING, self.id)
    
class PushNameInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.PUSH_NAME) + self.identifier.to_binary()

class PushFloatingInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_FLOATING,self.id)

class PopInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.POP)
    
class PushNullInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.PUSH_NULL)

class BuildListInstruction(Instruction):
    def __init__(self,count):
        self.count = count
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.BUILD_LIST,self.count)

class PushBooleanInstruction(Instruction):
    def __init__(self,value):
        self.value=value
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.PUSH_BOOLEAN,self.value)
    
class RetInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.RET)
    
class AssignNearestInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_NEAREST) + self.identifier.to_binary()
    
class AssignGlobalInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_GLOBAL) + self.identifier.to_binary()
    
class AssignLocalInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_LOCAL) + self.identifier.to_binary()
    
class AdditionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ADDITION)

class GreaterInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.GREATER)
class GreaterEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.GREATER_EQUAL)
class LesserInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.LESSER)
class LesserEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.LESSER_EQUAL)
class EqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.EQUAL)

class MultiplicationInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.MULTIPLICATION)

class DivisionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.DIVISION)

class SubtractionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.SUBTRACTION)

class PowInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.POW)

class NegationInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.NEGATION)

class BooleanNotInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.BOOLEAN_NOT)

class BooleanAndInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.BOOLEAN_AND)

class BooleanOrInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.BOOLEAN_OR)
    
class JumpInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        if fits_in_char(self.address.value):
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_RELATIVE, self.address.value)
            else:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_ABSOLUTE, self.address.value)
        else:
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_RELATIVE, self.address.value)
            else:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_ABSOLUTE, self.address.value)
        
class JumpIfFalseInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        if fits_in_char(self.address.value):
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_RELATIVE_IF_FALSE, self.address.value)
            else:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_ABSOLUTE_IF_FALSE, self.address.value)
        else:
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_RELATIVE_IF_FALSE, self.address.value)
            else:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_ABSOLUTE_IF_FALSE, self.address.value)
        
class TerminateInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.TERMINATE)
    
class CallFunctionInstruction(Instruction):
    def __init__(self,num_arguments):
        self.num_arguments = num_arguments
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.CALL_FUNCTION,self.num_arguments)    

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

        
    
    def to_binary(self):
        keys = list(self.statics.keys())
        keys.sort(key=lambda x:self.statics[x])
        binary = b""
        for type,value in keys:
            print(self.statics[(type,value)],value,type)
            if type == StaticType.NAME:
                s = value.encode("utf-8")
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
            elif type == StaticType.STRING:
                s = value.encode("utf-8")
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
            elif type == StaticType.FLOATING:
                s = str(value)
                binary += struct.pack("=BI"+str(len(s))+"s",type,len(s),s)
            elif type == StaticType.INTEGER64:
                binary += struct.pack("=Bq",type,value)
            elif type == StaticType.INTEGER32:
                binary += struct.pack("=Bi",type,value)
        return binary
        
        
    def length(self):
        return len(self.statics)

class ScopeLookup:
    def __init__(self):
        self.stack = []
    def push_scope(self):
        self.stack.append({})
    def pop_scope(self):
        self.stack.pop()
    def get_identifier(self,id):
        nearest_scope = self.stack[len(self.stack)-1]
        farest_scope = self.stack[0]
        if id not in nearest_scope:
            nearest_scope[id] = len(nearest_scope)
        if id not in farest_scope:
            farest_scope[id] = len(farest_scope)
        return farest_scope[id],nearest_scope[id]


            

class Compiler:
    def __init__(self):
        self.static_table = StaticTable()  
        self.scope_lookup = ScopeLookup()

    def get_identifier(self,name):
        global_id, local_id = self.scope_lookup.get_identifier(name)
        static_id = self.static_table.get_name_id(name)
        return Identifier(global_id,local_id,static_id)
    
    def resolve_addresses(self,code):
        for index,instruction in enumerate(code):
            if not hasattr(instruction,"address"): continue
            address = instruction.address
            if address.type == Address.UNRESOLVED_ABSOLUTE:
                instruction.address = address.resolve(index)

    def compile_function(self,function):
        self.scope_lookup.push_scope()
        compiled_body = []
        arguments = function.nodes[Function.NODE_ARGUMENTS].nodes[ArgumentList.NODE_ARGUMENT_LIST]
        for argument in reversed(arguments):
            compiled_body += [AssignNearestInstruction(self.get_identifier(argument.nodes[Argument.NODE_NAME].value))]
        
        # Pop of 'this'
        compiled_body += [PopInstruction()]
        
        compiled_body += self.compile_block(function.nodes[Function.NODE_BODY])
        
        compiled_body += [PushNullInstruction(),RetInstruction()]
        
        self.scope_lookup.pop_scope()
        
        code = [PushFunctionInstruction(UnresolvedAbsoluteAddress(3)),
                AssignNearestInstruction(self.get_identifier(function.nodes[Function.NODE_NAME].value)),
                JumpInstruction(RelativeAddress(len(compiled_body)+1))]
        code += compiled_body
        
        

        return code
    
    def compile_declaration(self,declaration):
        code = []
        
        scope_token = declaration.nodes[Declaration.NODE_SCOPE]

        is_const = Declaration.NODE_CONST in declaration.nodes
        
        if scope_token.value == KeywordToken.DIM:
            if is_const:
                assignment_instruction = AssignNearestConstInstruction
            else:
                assignment_instruction = AssignNearestInstruction
        elif scope_token.value == KeywordToken.GLOBAL:
            if is_const:
                assignment_instruction = AssignGlobalConstInstruction
            else:
                assignment_instruction = AssignGlobalInstruction
        elif scope_token.value == KeywordToken.LOCAL:
            if is_const:
                assignment_instruction = AssignLocalConstInstruction
            else:
                assignment_instruction = AssignLocalInstruction


            
        if Declaration.NODE_STATEMENTS in declaration.nodes:
            for stmt in declaration.nodes[Declaration.NODE_STATEMENTS]:
                code += self.compile_line_statement(stmt, assignment_instruction)        
        
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
    
        compiled_components.append((compiled_condition+[JumpIfFalseInstruction(None)],self.compile_block(statement.nodes[If.NODE_BODY])+[JumpInstruction(None)]))
        
        if If.NODE_ELSEIFS in statement.nodes:
            for elseif in statement.nodes[If.NODE_ELSEIFS]:
                elseif_compiled_condition = self.compile_expression(elseif.nodes[ElseIf.NODE_CONDITION])
                # Add dummy jump instruction to NEXT elseif/else/endif
                elseif_compiled_condition += [JumpIfFalseInstruction(None)]

                elseif_compiled_body = self.compile_block(elseif.nodes[ElseIf.NODE_BODY])
                # Add jump to endif
                elseif_compiled_body += [JumpInstruction(None)]

                compiled_components.append((elseif_compiled_condition,elseif_compiled_body))

        if If.NODE_ELSE in statement.nodes:
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
            print(number_of_instructions,consumed_instructions)
            body[-1].address = RelativeAddress(number_of_instructions - consumed_instructions)
            consumed_instructions += 1

            output_instructions += condition + body
        
        
    
        return output_instructions

    def compile_dountil(self,statement):
        code = []
        code += self.compile_block(statement.nodes[DoUntil.NODE_BODY])
        code += self.compile_expression(statement.nodes[DoUntil.NODE_CONDITION])
        code += [JumpIfFalseInstruction(RelativeAddress(-len(code)))]
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
            code += self.compile_expression(expression)
        code += [CallFunctionInstruction(len(call.nodes[Call.NODE_ARGUMENTS]))]
        return code
    
    def compile_name_assignment(self,assignment,name,assignment_instruction=AssignNearestInstruction):
        code = self.compile_expression(assignment.nodes[Assignment.NODE_VALUE_EXPRESSION])
        code += [assignment_instruction(name)]
        return code
    
    def compile_qualifiers(self,qualifiers):
        code = []
        while len(qualifiers) > 0:
            code += self.compile_qualifier(qualifiers.pop(0))
        return code
    
    def compile_line_statement(self,line_statement,assignment_instruction=AssignNearestInstruction):
        nodes = line_statement.nodes
        code = []
        
        ident = nodes[LineStatement.NODE_START]
        code.append(PushNameInstruction(self.get_identifier(ident.value)))
        
        qualifiers = nodes[LineStatement.NODE_QUALIFIERS]
        code += self.compile_qualifiers(qualifiers)
        
        if LineStatement.NODE_ASSIGNMENT in nodes:
            # remove last push_name instruction
            code.pop(0)
            assignment = nodes[LineStatement.NODE_ASSIGNMENT]
            code += self.compile_name_assignment(assignment, self.get_identifier(ident.value),assignment_instruction)  
        else:
            code += [PopInstruction()]       
        
        return code
    
    def compile_while_statement(self,while_statement):

        
        compiled_condition = self.compile_expression(while_statement.nodes[While.NODE_CONDITION])
        compiled_body = []
        if While.NODE_BODY in while_statement.nodes:
            compiled_body = self.compile_block(while_statement.nodes[While.NODE_BODY])
        
        code = []
        code += compiled_condition
        code += [JumpIfFalseInstruction(RelativeAddress(len(compiled_body)+2))]
        code += compiled_body
        code += [JumpInstruction(RelativeAddress(-(len(compiled_body)+len(compiled_condition)+1)))]
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
                 OperatorToken.BOOLEAN_OR: BooleanOrInstruction
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
        token = terminal.nodes[Terminal.NODE_TYPE]
        
        if token.type == Token.INTEGER:
            if token.value >= -(2**31) and token.value < 2**31:
                return [PushInteger32Instruction(self.static_table.get_integer32_id(token.value))]
            return [PushInteger64Instruction(self.static_table.get_integer64_id(token.value))]
        if token.type == Token.IDENTIFIER:
            return [PushNameInstruction(self.get_identifier(token.value))]
        if token.type == Token.STRING:
            return [PushStringInstruction(self.static_table.get_string_id(token.value))]   
        if token.type == Token.FLOATING:
            return [PushFloatingInstruction(self.static_table.get_floating_id(token.value))]     
        if token.type == Token.BOOLEAN:
            return [PushBooleanInstruction(token.value)]

    def compile_inline_list(self,inline_list):
        code = []
        elements = inline_list.nodes[InlineList.NODE_ELEMENTS]
        for element in elements:
            code += self.compile_expression(element)
        code += [BuildListInstruction(len(elements))]
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
        self.scope_lookup.push_scope()
        code = self.compile_block(program.nodes[Program.NODE_BLOCK]) + [TerminateInstruction()]
        self.resolve_addresses(code)
        return code
    


