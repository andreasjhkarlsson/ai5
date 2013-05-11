
from rd_parser import *
from lexer import *
from runtime_types import StaticType
from runtime_types import InstructionType
import struct


def fits_in_char(integer):
    return integer >= -128 and integer <= 127

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

class PushIntegerInstruction(Instruction):
    def __init__(self,integer):
        self.integer = integer
    def to_binary(self):
        if fits_in_char(self.integer):
            return self.to_binary_with_char_arg(InstructionType.PUSH_SMALL_INTEGER, self.integer)
        else:
            return self.to_binary_with_int64_arg(InstructionType.PUSH_LARGE_INTEGER, self.integer)

class PushStringInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_STRING, self.id)
    
class PushNameInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_NAME, self.id)

class AssignNameInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.ASSIGN_NAME, self.id)

class AdditionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ADDITION)

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
    
class JumpRelativeInstruction(Instruction):
    def __init__(self,offset):
        self.offset = offset
    def to_binary(self):
        if fits_in_char(self.offset):
            return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_RELATIVE, self.offset)
        else:
            return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_RELATIVE, self.offset)
        
class JumpRelativeIfFalseInstruction(Instruction):
    def __init__(self,offset):
        self.offset = offset
    def to_binary(self):
        if fits_in_char(self.offset):
            return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_RELATIVE_IF_FALSE, self.offset)
        else:
            return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_RELATIVE_IF_FALSE, self.offset)
        
class TerminateInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.TERMINATE)

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
    
    def to_binary(self):
        keys = list(self.statics.keys())
        keys.sort(key=lambda x:self.statics[x])
        binary = b""
        for type,value in keys:
            if type == StaticType.NAME:
                binary += struct.pack("BI"+str(len(value))+"s",type,len(value),value)
            elif type == StaticType.STRING:
                binary += struct.pack("BB"+str(len(value))+"s",type,len(value),value)
        return binary
        
        
    def length(self):
        return len(self.statics)

            

class Compiler:
    def __init__(self):
        self.static_table = StaticTable()

    def compile_statement(self,statement):
        return {Rule.LINE_STATEMENT: self.compile_line_statement,
                Rule.WHILE: self.compile_while_statement}[statement.nodes[0].type](statement.nodes[0])
    
    def compile_line_statement(self,line_statement):
        code = []
        
        ident = line_statement.nodes[0]
        
        if line_statement.nodes[1].type == Rule.ASSIGNMENT:
            assignment = line_statement.nodes[1]
            code += self.compile_expression(assignment.nodes[1])
            code.append(AssignNameInstruction(self.static_table.get_name_id(ident.value)))
            
        
        return code
    
    def compile_while_statement(self,while_statement):

        
        compiled_condition = self.compile_expression(while_statement.condition)
        compiled_body = []
        if while_statement.body:
            compiled_body = self.compile_block(while_statement.body)
        
        code = []
        code += compiled_condition
        code += [JumpRelativeIfFalseInstruction(len(compiled_body)+2)]
        code += compiled_body
        code += [JumpRelativeInstruction(-(len(compiled_body)+len(compiled_condition)+1))]
        return code
    
    def compile_operator(self,operator):
        token = operator.nodes[0]
        
        return [{OperatorToken.ADD: AdditionInstruction,
                 OperatorToken.SUBTRACT: SubtractionInstruction,
                 OperatorToken.MULTIPLY: MultiplicationInstruction,
                 OperatorToken.DIVIDE: DivisionInstruction,
                 OperatorToken.POW: PowInstruction
                 }[token.value]()]
        
        
        if token.value == OperatorToken.ADD:
            return [AdditionInstruction()]
        elif token.value == OperatorToken.MULTIPLY:
            return [MultiplicationInstruction()]
        
        # throw or something    
    def compile_unary_operator(self,unary):
        return [{OperatorToken.SUBTRACT:NegationInstruction,
                 OperatorToken.BOOLEAN_NOT: BooleanNotInstruction}[unary.nodes[0].value]()]
    
    def compile_terminal(self,terminal):
        token = terminal.nodes[0]
        
        if token.type == Token.INTEGER:
            return [PushIntegerInstruction(token.value)]
        if token.type == Token.IDENTIFIER:
            return [PushNameInstruction(self.static_table.get_name_id(token.value))]
        if token.type == Token.STRING:
            return [PushStringInstruction(self.static_table.get_string_id(token.value))]        
        
    def compile_factor(self,factor):
        rule = factor.nodes[0]
        code = []
        
        unary = None
        if rule.type == Rule.UNARY_OPERATOR:
            unary = self.compile_unary_operator(rule)
            rule = factor.nodes[1]
        
        if rule.type == Rule.TERMINAL:
            code = self.compile_terminal(rule)
        elif rule.type == Rule.EXPRESSION:
            code = self.compile_expression(rule)
        
        if unary:
            code += unary
            
        return code
    
    def compile_expression(self,expr):
        code = self.compile_factor(expr.nodes.pop(0))
        
        while expr.nodes:
            op = expr.nodes.pop(0)
            right_hand = expr.nodes.pop(0)
            if right_hand.type == Rule.FACTOR:
                code += self.compile_factor(right_hand)
            elif right_hand.type == Rule.EXPRESSION:
                code += self.compile_expression(right_hand)
            code += self.compile_operator(op)
        return code    
    
    def compile_block(self,block):
        code = []
        for stm in block.nodes:
            code += self.compile_statement(stm)
        return code
    
    def compile_program(self,program):
        return self.compile_block(program.nodes[0]) + [TerminateInstruction()]
    


