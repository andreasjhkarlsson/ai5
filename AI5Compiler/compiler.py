
from rd_parser import *
from lexer import *

class Instruction:
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)

class PushIntegerInstruction(Instruction):
    def __init__(self,integer):
        self.integer = integer
        
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

def compile_expression(expr):
    code = compile_factor(expr.nodes.pop(0))
    
    while expr.nodes:
        op = expr.nodes.pop(0)
        right_hand = expr.nodes.pop(0)
        if right_hand.type == Rule.FACTOR:
            code += compile_factor(right_hand)
        elif right_hand.type == Rule.EXPRESSION:
            code += compile_expression(right_hand)
        code += compile_operator(op)
    return code
    

def compile_factor(factor):
    rule = factor.nodes[0]
    code = []
    
    unary = None
    if rule.type == Rule.UNARY_OPERATOR:
        unary = compile_unary_operator(rule)
        rule = factor.nodes[1]
    
    if rule.type == Rule.TERMINAL:
        code = compile_terminal(rule)
    elif rule.type == Rule.EXPRESSION:
        code = compile_expression(rule)
    
    if unary:
        code += unary
        
    return code
    
def compile_unary_operator(unary):
    return [{OperatorToken.SUBTRACT:NegationInstruction,
             OperatorToken.BOOLEAN_NOT: BooleanNotInstruction}[unary.nodes[0].value]()]

def compile_terminal(terminal):
    token = terminal.nodes[0]
    if token.type == Token.INTEGER:
        return [PushIntegerInstruction(token.value)]

def compile_operator(operator):
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
    
def compile_line_statement(line_statement):
    return [123,123,123,123]

def compile_statement(statement):
    return {Rule.LINE_STATEMENT: compile_line_statement}[statement.nodes[0].type](statement.nodes[0])
    


tokens = lex_string("foo()")
parser = Parser(tokens)
e = parser.expectRule(Statement)

print_ast(e)

print(compile_statement(e))

