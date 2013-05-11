
from rd_parser import *
from lexer import *

class Instruction:
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)

class PushIntegerInstruction(Instruction):
    def __init__(self,integer):
        self.integer = integer

class PushNameInstruction(Instruction):
    def __init__(self,id):
        self.id = id

class AssignNameInstruction(Instruction):
    def __init__(self,id):
        self.id = id

class AdditionInstruction(Instruction):
    pass

class MultiplicationInstruction(Instruction):
    pass

class DivisionInstruction(Instruction):
    pass

class SubtractionInstruction(Instruction):
    pass

class PowInstruction(Instruction):
    pass

class NegationInstruction(Instruction):
    pass

class BooleanNotInstruction(Instruction):
    pass

class JumpRelativeInstruction(Instruction):
    def __init__(self,offset):
        self.offset = offset
        
class JumpRelativeIfFalseInstruction(Instruction):
    def __init__(self,offset):
        self.offset = offset;

class SymbolTable:
    def __init__(self):
        self.symbols = {}
    def get_symbol_id(self,symbol):
        if symbol not in self.symbols:
            self.symbols[symbol] = len(self.symbols)
        return self.symbols[symbol]
            

class Compiler:
    def __init__(self):
        self.symbol_table = SymbolTable()

    def compile_statement(self,statement):
        return {Rule.LINE_STATEMENT: self.compile_line_statement,
                Rule.WHILE: self.compile_while_statement}[statement.nodes[0].type](statement.nodes[0])
    
    def compile_line_statement(self,line_statement):
        code = []
        
        ident = line_statement.nodes[0]
        
        if line_statement.nodes[1].type == Rule.ASSIGNMENT:
            assignment = line_statement.nodes[1]
            code += self.compile_expression(assignment.nodes[1])
            code.append(AssignNameInstruction(self.symbol_table.get_symbol_id(ident.value)))
            
        
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
            return [PushNameInstruction(self.symbol_table.get_symbol_id(token.value))]
        
        
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
        return self.compile_block(program.nodes[0])
    
tokens = lex_string("""
while 1+2
a = 1+2+3+4+5+6+7+8+9+10
wend
""")
parser = Parser(tokens)
e = parser.expectRule(Program)

print_ast(e)

compiler = Compiler()

print(compiler.compile_program(e))

