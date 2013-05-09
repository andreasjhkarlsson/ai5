
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
    if rule.type == Rule.TERMINAL:
        return compile_terminal(rule)
    if rule.type == Rule.EXPRESSION:
        return compile_expression(rule)

def compile_terminal(terminal):
    token = terminal.nodes[0]
    if token.type == Token.INTEGER:
        return [PushIntegerInstruction(token.value)]

def compile_operator(operator):
    token = operator.nodes[0]
    
    operator_instructions = {
                 OperatorToken.ADD: AdditionInstruction,
                 OperatorToken.SUBTRACT: SubtractionInstruction,
                 OperatorToken.MULTIPLY: MultiplicationInstruction,
                 OperatorToken.DIVIDE: DivisionInstruction
                 }
    return [operator_instructions[token.value]()]
    
    
    if token.value == OperatorToken.ADD:
        return [AdditionInstruction()]
    elif token.value == OperatorToken.MULTIPLY:
        return [MultiplicationInstruction()]
    
    # throw or something



tokens = lex_string("1/2/3")
parser = Parser(tokens)
e = parser.acceptRule(Expression)

print_ast(e)

print(compile_expression(e))

