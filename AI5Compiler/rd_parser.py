from collections import deque
import lexer
from lexer import Token, KeywordToken, OperatorToken

class Parser:
    
    def generate_parse_error(self,msg):
        raise Exception(msg)
    
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
    def skip_to_newline(self):
        while self.current.type != Token.NEWLINE and self.current.type != Token.EOF:
            self.next()
        self.back()
        
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
            self.generate_parse_error("Expected "+token_type+" but found "+self.current.type+" with value "+str(self.current.value))
        if value != None and self.current.value != value:
            self.generate_parse_error("Expected: "+token_type+" with value: "+str(value))
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
            self.generate_parse_error("Expected "+rule_class.__name__+" but found "+str(self.current))
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
    EXIT = "rule_exit"
    EXITLOOP = "rule_exitloop"
    CONTINUELOOP = "rule_continueloop"
    DECLARATION = "rule_declaration"
    ENUM = "rule_enum"
    ENUMLIST = "rule_enumlist"
    ENUM_CONSTANT = "rule_enum_constant"
    ENUM_STEP = "rule_enum_step"
    ASSIGNMENT = "rule_assignment"
    REDIM = "rule_redim"
    LINE_STATEMENT = "rule_line_statement"
    STATEMENT = "rule_statement"
    WITH = "rule_with"
    RETURN = "rule_return"
    DO_UNTIL = "rule_do_until"
    FOR = "rule_for"
    FOR_TO = "rule_for_to"
    FOR_IN = "rule_for_in"
    QUALIFIER = "rule_qualifier"
    ARGUMENT_LIST = "rule_argument_list"
    ARGUMENT = "rule_argument"
    FUNCTION = "rule_function"
    BLOCK = "rule_block"
    WHILE = "rule_while"
    SWITCH = "rule_switch"
    SWITCH_CASE = "rule_case"
    SWITCH_CONDITION = "rule_switch_condition"
    IF = "rule_if"
    ELSEIF = "rule_elseif"
    ELSE = "rule_else"
    DIRECTIVE = "rule_directive"
    INCLUDE = "rule_include"
    UNARY_OPERATOR = "rule_unary_operator"
    FACTOR = "rule_factor"
    INLINE_LIST = "rule_inline_list"
    CALL = "rule_call"
    PROPERTY = "rule_property"
    LIST_INDEXING = "rule_list_indexing"
    TERMINAL = "rule_terminal"
    BINARY_OPERATOR = "rule_binary_operator"
    EXPRESSION = "rule_expression"
    PROGRAM = "rule_program"
    
    
    def __init__(self,nodes):
        self.nodes = nodes
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.nodes)
    
class Exit(Rule):
    type = Rule.EXIT
    @staticmethod
    def match(parser):
        if parser.accept(Token.KEYWORD,KeywordToken.EXIT):
            return Exit([parser.current])
        return None
class ExitLoop(Rule):
    type = Rule.EXITLOOP
    @staticmethod
    def match(parser):
        if parser.accept(Token.KEYWORD,KeywordToken.EXITLOOP):
            nodes = [parser.current]
            if parser.accept(Token.INTEGER):
                nodes.append(parser.current)
            return ExitLoop(nodes)
        return None
    
class ContinueLoop(Rule):
    type = Rule.CONTINUELOOP
    @staticmethod
    def match(parser):
        if parser.accept(Token.KEYWORD,KeywordToken.CONTINUELOOP):
            nodes = [parser.current]
            if parser.accept(Token.INTEGER):
                nodes.append(parser.current)
            return ContinueLoop(nodes)
        return None

class Declaration(Rule):
    type = Rule.DECLARATION
    @staticmethod
    def match(parser): 
        if not (parser.accept(Token.KEYWORD,KeywordToken.DIM) or 
                parser.accept(Token.KEYWORD,KeywordToken.LOCAL) or
                parser.accept(Token.KEYWORD,KeywordToken.GLOBAL)):
            return None
        nodes = [parser.current]
        if parser.acceptRule(Enum):
            nodes.append(parser.matched_rule)
            return Declaration(nodes)
        
        if parser.accept(Token.KEYWORD,KeywordToken.CONST):
            nodes.append(parser.current)
        
        
        nodes.append(parser.expectRule(LineStatement))
        
        while parser.accept(Token.COMMA):
            nodes.append(parser.expectRule(LineStatement))
        
        return Declaration(nodes)


class Enum(Rule):
    type = Rule.ENUM
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.ENUM):
            return None
        nodes = []
        if parser.acceptRule(EnumStep):
            nodes.append(parser.matched_rule)
        nodes.append(parser.expectRule(EnumList))
        return Enum(nodes)

class EnumList(Rule):
    type = Rule.ENUMLIST
    @staticmethod
    def match(parser):
        if not parser.acceptRule(EnumConstant):
            return None
        nodes = [parser.matched_rule]
        while parser.accept(Token.COMMA):
            nodes.append(parser.expectRule(EnumConstant))
        return EnumList(nodes)


class EnumConstant(Rule):
    type = Rule.ENUM_CONSTANT
    @staticmethod
    def match(parser):
        if not parser.accept(Token.IDENTIFIER):
            return None
        nodes = [parser.current]
        if parser.acceptRule(Assignment):
            nodes.append(parser.matched_rule)
        return EnumConstant(nodes)

        
        
class EnumStep(Rule):
    type = Rule.ENUM_STEP
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.STEP):
            return None
        nodes = []
        if parser.accept(Token.OPERATOR,OperatorToken.ADD) or parser.accept(Token.OPERATOR,OperatorToken.SUBTRACT) or parser.accept(Token.OPERATOR,OperatorToken.MULTIPLY):
            nodes.append(parser.current)
        nodes.append(parser.expect(Token.INTEGER))
        return EnumStep(nodes)
            

class Assignment(Rule):
    type = Rule.ASSIGNMENT
    @staticmethod
    def match(parser):
        for operator in OperatorToken.ASSIGNMENT_OPERATORS:
            if parser.accept(Token.OPERATOR,operator):
                nodes = [parser.current]
                nodes.append(parser.expectRule(Expression))
                
                return Assignment(nodes)
        return None
 
 
class ReDim(Rule):
    type = Rule.REDIM
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.REDIM):
            return None
        nodes = [parser.expectRule(Terminal)]
        while parser.acceptAnyRule([Property,ListIndexing]):
            nodes.append(parser.matched_rule)
        return ReDim(nodes)
     
 
class LineStatement(Rule):
    type = Rule.LINE_STATEMENT
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
    type = Rule.STATEMENT
    @staticmethod
    def match(parser):
        if parser.acceptAnyRule([With,ReDim,Enum,Return,DoUntil,For,Include,Directive,Exit,ExitLoop,ContinueLoop,Declaration,Function,While,If,Switch,LineStatement]):
            return Statement([parser.matched_rule])
        #if parser.acceptRule(Expression):
        #    return Statement([parser.matched_rule])
        
        return None
    
    
class With(Rule):
    type = Rule.WITH
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.WITH):
            return None
        nodes = [parser.expectRule(Expression)]
        parser.expect(Token.NEWLINE)
        while parser.accept(Token.DOT):
            nodes.append(parser.expectRule(LineStatement))
            parser.expect(Token.NEWLINE)
        parser.expect(Token.KEYWORD,KeywordToken.ENDWITH)
        return With(nodes)
            

class Return(Rule):
    type = Rule.RETURN
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.RETURN):
            return None
        nodes = []
        if parser.acceptRule(Expression):
            nodes.append(parser.matched_rule)
        return Return(nodes)  
    
class DoUntil(Rule):
    type = Rule.DO_UNTIL
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.DO):
            return None
        parser.expect(Token.NEWLINE)
        nodes = [parser.expectRule(Block)]
        parser.expect(Token.KEYWORD,KeywordToken.UNTIL)
        nodes.append(parser.expectRule(Expression))
        return DoUntil(nodes)    

class For(Rule):
    type = Rule.FOR
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.FOR):
            return None
        nodes = [parser.expect(Token.IDENTIFIER)]
        if parser.acceptRule(ForTo):
            nodes.append(parser.matched_rule)
        else:
            nodes.append(parser.expectRule(ForIn))
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes.append(parser.matched_rule)
        parser.expect(Token.KEYWORD,KeywordToken.NEXT)
        return For(nodes)
            
        
class ForTo(Rule):
    type = Rule.FOR_TO
    @staticmethod
    def match(parser):
        if not parser.accept(Token.OPERATOR,OperatorToken.EQUAL):
            return None
        nodes = [parser.expectRule(Expression)]
        parser.expect(Token.KEYWORD,KeywordToken.TO)
        nodes.append(parser.expectRule(Expression))
        if parser.accept(Token.KEYWORD,KeywordToken.STEP):
            nodes.append(parser.expectRule(Expression))
        return ForTo(nodes)    
        
        
        
class ForIn(Rule):
    type = Rule.FOR_IN
    @staticmethod
    def match(parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.IN):
            return None
        return ForIn([parser.expectRule(Expression)])       


class Qualifier(Rule):
    type = Rule.QUALIFIER
    @staticmethod
    def match(parser):
        if parser.acceptAnyRule([Property,Call,ListIndexing]):
            return Qualifier([parser.matched_rule])
        return None    
            
class ArgumentList(Rule):
    type = Rule.ARGUMENT_LIST
    @staticmethod
    def match(parser):
        if not parser.accept(Token.LEFT_PAREN):
            return None
    
        nodes = []
        
        if parser.acceptRule(Argument):
            nodes.append(parser.current)
            while parser.accept(Token.COMMA):
                nodes.append(parser.expectRule(Argument))      
        
        parser.expect(Token.RIGHT_PAREN) 
        return ArgumentList(nodes)   
    
class Argument(Rule):
    type = Rule.ARGUMENT
    @staticmethod
    def match(parser):
        nodes = []
        if parser.accept(Token.KEYWORD,KeywordToken.BYREF):
            nodes.append(parser.current)
            nodes.append(parser.expect(Token.IDENTIFIER))
        elif parser.accept(Token.IDENTIFIER):
            nodes.append(parser.current)
        else:
            return None
        if parser.accept(Token.OPERATOR,OperatorToken.EQUAL):
            nodes.append(parser.expectRule(Terminal))
        return Argument(nodes)
            

class Function(Rule):
    type = Rule.FUNCTION
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
    type = Rule.BLOCK
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
    
    def __init__(self,nodes):
        super(While,self).__init__(nodes)
        self.condition = nodes[0]
        self.body = None
        if len(nodes) == 2:
            self.body = nodes[1]
    
    type = Rule.WHILE
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
    type = Rule.SWITCH
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
    type = Rule.SWITCH_CONDITION
    @staticmethod
    def match(parser):
        if not parser.acceptRule(Expression):
            return None
        nodes = [parser.matched_rule]
        if parser.accept(Token.KEYWORD,KeywordToken.TO):
            nodes.append(parser.expectRule(Expression))
        return SwitchCondition(nodes)
    
class SwitchCase(Rule):  
    type = Rule.SWITCH_CASE  
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
    type = Rule.IF
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
            nodes.append(parser.expectRule(Statement))    
        return If(nodes)

class ElseIf(Rule):
    type = Rule.ELSEIF
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
    type = Rule.ELSE
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
    type = Rule.DIRECTIVE
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
            parser.skip_to_newline()
            return Directive([token])
        
        return None
    
class Include(Rule):
    type = Rule.INCLUDE
    @staticmethod
    def match(parser):
        if parser.accept(Token.INCLUDE_FILE):
            token = parser.current
            return Include([token])
        
        
class UnaryOperator(Rule):
    type = Rule.UNARY_OPERATOR
    @staticmethod
    def match(parser):
        for operator in OperatorToken.UNARY_OPERATORS:  
            if parser.accept(Token.OPERATOR,operator):
                return UnaryOperator([parser.current])


class Factor(Rule):
    type = Rule.FACTOR
    @staticmethod
    def match(parser):
        nodes = []
        
        
        while parser.acceptRule(UnaryOperator):
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
    type = Rule.INLINE_LIST
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
    type = Rule.CALL
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
    type = Rule.PROPERTY
    @staticmethod
    def match(parser):
        if not parser.accept(Token.DOT):
            return None
        return Property([parser.expect(Token.IDENTIFIER)])


class ListIndexing(Rule):
    type = Rule.LIST_INDEXING
    @staticmethod
    def match(parser):
        if not parser.accept(Token.LEFT_BRACKET):
            return None
        node = parser.expectRule(Expression)
        parser.expect(Token.RIGHT_BRACKET)
        return ListIndexing([node])

class Terminal(Rule):
    type = Rule.TERMINAL
    @staticmethod
    def match(parser):
        accepted = [Token.BOOLEAN,Token.MACRO,Token.IDENTIFIER,Token.INTEGER,Token.STRING,Token.FLOATING]
        for token_type in accepted:
            if parser.accept(token_type):
                return Terminal([parser.current])
        return None


class BinaryOperator(Rule):
    
    type = Rule.BINARY_OPERATOR
    
    PRECEDENCE_LEVELS = [[OperatorToken.BOOLEAN_AND,OperatorToken.BOOLEAN_OR],
              [OperatorToken.GREATER,OperatorToken.LESSER,OperatorToken.GREATER_EQUAL,
                                    OperatorToken.LESSER_EQUAL,OperatorToken.EQUAL,
                                    OperatorToken.NOT_EQUAL,OperatorToken.STRONG_EQUAL],
              [OperatorToken.CONCAT],
              [OperatorToken.ADD,OperatorToken.SUBTRACT],
              [OperatorToken.MULTIPLY,OperatorToken.DIVIDE],
              [OperatorToken.POW]
              ]
    @staticmethod
    def match(parser):
        for operator in OperatorToken.BINARY_OPERATORS:
            if parser.accept(Token.OPERATOR,operator):
                return BinaryOperator([parser.current])
        return None
        
    def precedence_level(self):
        for index,level in enumerate(BinaryOperator.PRECEDENCE_LEVELS):
            if self.nodes[0].value in level:
                return index
        raise Exception("No precedence level for token "+str(self.value))
    def is_left_associative(self):
        return self.nodes[0].value != OperatorToken.POW

    
class Expression(Rule):
    
    type = Rule.EXPRESSION
    
    def __init__(self,nodes):
        super(Expression,self).__init__(nodes)
        self.first_factor = nodes[0]
        if len(nodes) == 3:
            self.operator = nodes[1]
            self.second_factor = nodes[2]
        else:
            self.operator = None
            self.second_factor = None
    
    @staticmethod
    def climb_precedence(nodes):
        nodes = deque(nodes)
        
        
        def get_tree(min_prec=0,rec=0):
            
            
            lhs = nodes.popleft()
            
            tree = [lhs]
            #   while cur token is a binary operator with precedence >= min_prec:
            while True:
                
                if len(nodes) == 0:
                    return Expression(tree)
                
                op = nodes[0]
                

                prec = op.precedence_level()
                if prec < min_prec:
                    return Expression(tree)
                
                nodes.popleft()
                
                next_min_prec = prec
                if op.is_left_associative():
                    next_min_prec += 1
                
                rhs = get_tree(next_min_prec,rec+1)
                
                if len(rhs.nodes) == 1:
                    rhs = rhs.nodes[0]
                
                tree.append(op)
                tree.append(rhs)
                
                
                
        
        return get_tree().nodes
    
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

        return Expression(Expression.climb_precedence(nodes))
            

class Program(Rule):
    type = Rule.PROGRAM
    @staticmethod
    def match(parser):
        nodes = []
        if parser.acceptRule(Block):
            nodes.append(parser.matched_rule)
        parser.expect(Token.EOF)
        return Program(nodes)
    
    
def print_ast(node,depth=0):
    print(" "*depth,node.__class__.__name__,end="")
    if "nodes" in node.__dict__:
        print("")
        for child in node.nodes:
            print_ast(child,depth+1)
    else:
        print(" "+str(node.value))
        
   


#test_code = """if 1+2==3 then a = 10"""
#test_code = open("test.au3").read()
#tokens = lexer.lex_string(test_code)
#parser = Parser(tokens)



#p = parser.acceptRule(Program)

#print_ast(p)    
    