from collections import deque
import lexer
from lexer import Token, KeywordToken

class Parser:
    def __init__(self,tokens):
        self.tokens = deque(tokens)
        self.current = None
        self.matched_rule = None
    def next(self):
        self.previous = self.current
        self.current = self.tokens.popleft()
        print("Current token: "+self.current.type)
    def back(self):
        self.tokens.appendleft(self.current)
        self.current = self.previous
    
    def accept(self,token_type,value=None):
        self.next()
        if self.current.type != token_type:
            self.back()
            return None
        if value != None and value != self.current.value:
            self.back()
            return None
        return self.current
    def expect(self,token_type,value=None):
        self.next()
        if self.current.type != token_type:
            raise Exception("Expected token of type: "+token_type+" was: "+self.current.type)
        if value != None and self.current.value != value:
            raise Exception("Expected token of type: "+token_type+" and value: "+str(value))
        return self.current
    def acceptRule(self,rule_class):
        self.matched_rule = rule_class.match(self)
        return self.matched_rule
    def expectRule(self,rule_class):
        self.matched_rule = rule_class.match(self)
        if not self.matched_rule:
            raise Exception("Expected :"+type(rule_class))
    def expectNewline(self):
        if self.accept(Token.EOF):
            return None
        self.expect(Token.NEWLINE)
        

class Rule:
    def __repr__(self):
        return "%s(%r)" % (self.__class__, self.__dict__)

class Function(Rule):
    def __init__(self,name,arguments,body):
        self.name = name
        self.arguments = arguments
        self.body = body
    @staticmethod
    def match(parser):
        if parser.accept(Token.KEYWORD,KeywordToken.FUNC):
            func_name = parser.expect(Token.IDENTIFIER)
            parser.expect(Token.LEFT_PAREN)
            arguments = []
            while parser.accept(Token.IDENTIFIER):
                arguments.append(parser.current)
                if not parser.accept(Token.COMMA):
                    break   
            parser.expect(Token.RIGHT_PAREN)
            parser.expectNewline() 
            
            # Match a function body here
            
            parser.expect(Token.KEYWORD,KeywordToken.ENDFUNC)
            parser.expectNewline()
            
            return Function(func_name,arguments,None)
        else:
            return None
        
class Block(Rule):
    def __init__(self,blocks):
        self.blocks = blocks
    @staticmethod
    def match(parser):
        pass

class Directive(Rule):
    def __init__(self,directive_token,argument=None):
        self.argument = argument
        self.directive_token = directive_token
    @staticmethod
    def match(parser):
        argument = None
        directive = None
        
        if parser.accept(Token.DIRECTIVE, "#ONAUTOITSSTARTREGISTER"):
            directive = parser.current
            argument = parser.expect(Token.STRING)
            parser.expectNewline()
            return Directive(directive,argument)
        if parser.accept(Token.DIRECTIVE):
            token = parser.current
            parser.expectNewline()
            return Directive(token)
        
        return None
    
class Include(Rule):
    def __init__(self,include_token):
        self.include_token = include_token
        
    @staticmethod
    def match(parser):
        if parser.accept(Token.INCLUDE_FILE):
            token = parser.current
            parser.expectNewline()
            return Include(token)
    
        

class Program(Rule):
    def __init__(self,directives,block):
        self.directives = directives
        self.block = block
    @staticmethod
    def match(parser):
        directives = []
        while parser.acceptRule(Include) or parser.acceptRule(Directive):
            directives.append(parser.matched_rule)
        return Program(directives,parser.acceptRule(Block))
        
        
    
    
            
    
test_code = "#include <hej.test>\n#requireadmin\n"
parser = Parser(lexer.lex_string(test_code))


p = parser.acceptRule(Program)
print(p.directives)

    
    