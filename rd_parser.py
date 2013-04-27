from collections import deque
import lexer
from lexer import Token, KeywordToken

class Parser:
    def __init__(self,tokens):
        self.tokens = deque(tokens)
        self.current = None
        self.matched_rule = None
        self.discarded = deque()
    def next(self):
        self.discarded.append(self.current)
        self.current = self.tokens.popleft()
    def back(self):
        self.tokens.appendleft(self.current)
        self.current = self.discarded.pop()
        
    def accept_multiple_tokens(self,tokens):
        matched = []
        for token in tokens:
            if len(token) == 1:
                token.append(None)
            if self.accept(token[0], token[1]):
                matched.append(self.current)
            else:
                for match in matched: self.back()
                return None
        return matched

    def accept(self,token_type,value=None):
        self.next()
        if self.current.type != token_type:
            self.back()
            return None
        if value != None and value != self.current.value:
            self.back()
            return None
        return self.current
    # Consumes and returns expected token. Throws if not equal.
    def expect(self,token_type,value=None):
        self.next()
        if self.current.type != token_type:
            raise Exception("Expected "+token_type+" but found "+self.current.type)
        if value != None and self.current.value != value:
            raise Exception("Expected: "+token_type+" with value: "+str(value))
        return self.current
    # Match a rule and return None if it doesn't match.
    def acceptRule(self,rule_class):
        self.matched_rule = rule_class.match(self)
        return self.matched_rule
    # Try matching any of the supplied rules.
    def acceptAnyRule(self,rules):
        for rule in rules:
            if self.acceptRule(rule):
                return self.matched_rule
        return None
    # Expect that the next token(s) are matched by rule.
    def expectRule(self,rule_class):
        self.matched_rule = rule_class.match(self)
        if not self.matched_rule:
            raise Exception("Expected "+rule_class.__name__)
        return self.matched_rule
    # Expect token of newline type.
    def expectNewline(self):
        self.expect(Token.NEWLINE)
    # Check if next token is EOF without consuming token.
    def isNextEOF(self):
        self.next()
        result = self.current.type == Token.EOF
        self.back()
        return result
        

class Rule:
    def __init__(self,nodes):
        self.nodes = nodes
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.nodes)
    
class Declaration(Rule):
    @staticmethod
    def match(parser): 
        if not (parser.accept(Token.KEYWORD,KeywordToken.DIM) or 
                parser.accept(Token.KEYWORD,KeywordToken.LOCAL) or
                parser.accept(Token.KEYWORD,KeywordToken.GLOBAL)):
            return None
        nodes = [parser.current]
        nodes.append(parser.expectRule(LineStatement))
        return Declaration(nodes)
    

class Assignment(Rule):
    operators = ["=","+=","-=","*=","/=","&="]
    @staticmethod
    def match(parser):
        for operator in Assignment.operators:
            if parser.accept(Token.OPERATOR,operator):
                nodes = [parser.current]
                nodes.append(parser.expectRule(Expression))
                return Assignment(nodes)
        return None
 
class LineStatement(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.IDENTIFIER):
            return None
        nodes = [parser.current]
        while parser.acceptRule(Qualifier):
            nodes.append(parser.matched_rule)
        if parser.acceptRule(Assignment):
            nodes.append(parser.matched_rule)
        return LineStatement(nodes)

class Statement(Rule):
    @staticmethod
    def match(parser):
        if parser.acceptAnyRule([Declaration,Function,While,If,Switch,LineStatement]):
            return Statement([parser.matched_rule])
        #if parser.acceptRule(Expression):
        #    return Statement([parser.matched_rule])
        
        return None

class Qualifier(Rule):
    @staticmethod
    def match(parser):
        if parser.acceptAnyRule([Property,Call,ListIndexing]):
            return Qualifier([parser.matched_rule])
        return None    
            
class ArgumentList(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.LEFT_PAREN):
            return None
        
        nodes = []
        
        
        if parser.accept(Token.IDENTIFIER):
            nodes.append(parser.current)
            while parser.accept(Token.COMMA):
                nodes.append(parser.expect(Token.IDENTIFIER))      
        
        parser.expect(Token.RIGHT_PAREN) 
        return ArgumentList(nodes)   

class Function(Rule):
    @staticmethod
    def match(parser):
        if parser.accept(Token.KEYWORD,KeywordToken.FUNC):
            nodes = []
            nodes.append(parser.expect(Token.IDENTIFIER))
            nodes.append(parser.expectRule(ArgumentList))
            parser.expectNewline() 
            
            if parser.acceptRule(Block):
                nodes.append(parser.matched_rule)
            
            parser.expect(Token.KEYWORD,KeywordToken.ENDFUNC)
            
            return Function(nodes)
        else:
            return None
        
class Block(Rule):
    @staticmethod
    def match(parser):
        nodes = []
        something_matched = False
        while True:
            if parser.accept(Token.NEWLINE):
                something_matched = True
                continue
            if parser.acceptRule(Statement):
                something_matched = True
                nodes.append(parser.matched_rule)
                if not parser.isNextEOF():
                    parser.expect(Token.NEWLINE)
                continue
            break
        if something_matched: return Block(nodes)
        return None
        

class While(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.WHILE):
            return None
        nodes = []
        nodes.append(parser.expectRule(Expression))
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes.append(parser.matched_rule)
        parser.expect(Token.KEYWORD,KeywordToken.WEND)
        return While(nodes)




class Switch(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.SWITCH):
            return None
        nodes = []
        nodes.append(parser.expectRule(Expression))
        parser.expect(Token.NEWLINE)
        while parser.acceptRule(SwitchCase):
            nodes.append(parser.matched_rule)
        parser.expect(Token.KEYWORD,KeywordToken.ENDSWITCH)
        return Switch(nodes)
    
    
class SwitchCondition(Rule):
    @staticmethod
    def match(parser):
        if not parser.acceptRule(Expression):
            return None
        nodes = [parser.matched_rule]
        if parser.accept(Token.KEYWORD,KeywordToken.TO):
            nodes.append(parser.expectRule(Expression))
        return SwitchCondition(nodes)
    
class SwitchCase(Rule):    
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.CASE):
            return None
        
        nodes = [parser.expectRule(SwitchCondition)]
        while parser.accept(Token.COMMA):
            nodes.append(parser.expectRule(SwitchCondition))
        
        parser.expect(Token.NEWLINE)
        
        nodes.append(parser.expectRule(Block))
        
        return SwitchCase(nodes)
            
        
        
        
class If(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.IF):
            return None
        nodes = []
        nodes.append(parser.expectRule(Expression))
        parser.expect(Token.KEYWORD,KeywordToken.THEN)
        if parser.accept(Token.NEWLINE):
            if parser.acceptRule(Block):
                nodes.append(parser.matched_rule)
            while parser.acceptRule(ElseIf):
                nodes.append(parser.matched_rule)
            if parser.acceptRule(Else):
                nodes.append(parser.matched_rule)
            parser.expect(Token.KEYWORD,KeywordToken.ENDIF)
        else:
            # TODO: Only allow single line statement
            nodes.append(parser.expectRule(Statement))    
        return If(nodes)

class ElseIf(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.ELSEIF):
            return None 
        nodes = [parser.expectRule(Expression)]
        parser.expect(Token.KEYWORD,KeywordToken.THEN)
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes.append(parser.matched_rule)
        return ElseIf(nodes)

class Else(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.ELSE):
            return None 
        nodes = []
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes.append(parser.matched_rule)
        return Else(nodes)

class Directive(Rule):
    @staticmethod
    def match(parser):
        argument = None
        directive = None
        
        if parser.accept(Token.DIRECTIVE, "#ONAUTOITSSTARTREGISTER"):
            directive = parser.current
            argument = parser.expect(Token.STRING)
            parser.expectNewline()
            return Directive([directive,argument])
        if parser.accept(Token.DIRECTIVE):
            token = parser.current
            parser.expectNewline()
            return Directive([token])
        
        return None
    
class Include(Rule):
    @staticmethod
    def match(parser):
        if parser.accept(Token.INCLUDE_FILE):
            token = parser.current
            parser.expectNewline()
            return Include([token])
        
        
class UnaryOperator(Rule):
    @staticmethod
    def match(parser):
        if parser.accept(Token.OPERATOR,"not") or parser.accept(Token.OPERATOR,"-"):
            return UnaryOperator([parser.current])



class BinaryOperator(Rule):
    operators = [">=","<=","<>","<",">","-","=","*","+","-","/","&","==","^","and","or"]
    @staticmethod
    def match(parser):
        for operator in BinaryOperator.operators:
            if parser.accept(Token.OPERATOR,operator):
                return BinaryOperator([parser.current])
        return None

class Factor(Rule):
    @staticmethod
    def match(parser):
        nodes = []
        
        
        if parser.acceptRule(UnaryOperator):
            nodes.append(parser.matched_rule)
            
        
        
        if parser.acceptAnyRule([Terminal,InlineList]):
            nodes.append(parser.matched_rule)
        elif parser.accept(Token.LEFT_PAREN):
            nodes.append(parser.expectRule(Expression))
            parser.expect(Token.RIGHT_PAREN)
        else:
            return None
        while parser.acceptRule(Qualifier):
            nodes.append(parser.matched_rule)
        return Factor(nodes)

                
                
        
class InlineList(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.LEFT_BRACKET):
            return None
        nodes = []
        if parser.acceptRule(Expression):
            nodes.append(parser.matched_rule)
            while parser.accept(Token.COMMA):
                nodes.append(parser.expectRule(Expression))

        parser.expect(Token.RIGHT_BRACKET)
        return InlineList(nodes)
        
            

class Call(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.LEFT_PAREN):
            return None
        nodes = []
        if parser.acceptRule(Expression):
            nodes.append(parser.matched_rule)
            while parser.accept(Token.COMMA):
                nodes.append(parser.expectRule(Expression))
        parser.expect(Token.RIGHT_PAREN)
        return Call(nodes)
        

class Property(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.DOT):
            return None
        return Property([parser.expect(Token.IDENTIFIER)])


class ListIndexing(Rule):
    @staticmethod
    def match(parser):
        if not parser.accept(Token.LEFT_BRACKET):
            return None
        node = parser.expectRule(Expression)
        parser.expect(Token.RIGHT_BRACKET)
        return ListIndexing([node])

class Terminal(Rule):
    @staticmethod
    def match(parser):
        accepted = [Token.BOOLEAN,Token.MACRO,Token.IDENTIFIER,Token.INTEGER,Token.STRING,Token.FLOATING]
        for token_type in accepted:
            if parser.accept(token_type):
                return Terminal([parser.current])
        return None

    
class Expression(Rule):
    @staticmethod
    def match(parser):
        nodes = []
        
        if parser.acceptRule(Factor):
            nodes.append(parser.matched_rule)
        else:
            return None

        while parser.acceptRule(BinaryOperator):
            nodes.append(parser.matched_rule)
            nodes.append(parser.expectRule(Factor))            

        return Expression(nodes)
            

class Program(Rule):
    @staticmethod
    def match(parser):
        nodes = []
        while parser.acceptAnyRule([Include,Directive]):
            nodes.append(parser.matched_rule)
        if parser.acceptRule(Block):
            nodes.append(parser.matched_rule)
        parser.expect(Token.EOF)
        return Program(nodes)
    
    
def print_ast(node,depth=0):
    print("  "*depth,node.__class__.__name__,end="")
    if "nodes" in node.__dict__:
        print("")
        for child in node.nodes:
            print_ast(child,depth+1)
    else:
        print(" "+str(node.value))
        
   

test_code = """
if 2 * (1+3) then derp.herp()
"""
parser = Parser(lexer.lex_string(test_code))


p = parser.acceptRule(Program)
print_ast(p)

    
    