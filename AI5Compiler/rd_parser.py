from collections import deque
import lexer
from lexer import Token, KeywordToken, OperatorToken
from copy import copy

class ParseError(Exception):
    def __init__(self,message,source):
        self.message = message;
        self.source = source

class Name:
    DECLARED = "declared"
    REFERENCED = "referenced"
    def __init__(self,name,type):
        self.name = name
        self.type = type
        self.closed = False

class Scope:
    def __init__(self):
        self.names = {}
        self.subscopes = []
    def add_name(self,new_name,type):
        if new_name in self.names:
            old_name_obj = self.names[new_name]
            if old_name_obj.type == Name.REFERENCED and type == Name.DECLARED:
                old_name_obj.type = Name.DECLARED
        else:
            self.names[new_name] = Name(new_name,type)
    def get_name(self,name):
        return self.names[name]
    def has_name(self,name):
        return name in self.names
    def add_subscope(self,subscope):
        self.subscopes.append(subscope)


class Parser:

    def __init__(self,tokens):
        self.tokens = deque(tokens)
        self.current = None
        self.matched_rule = None
        self.discarded = deque()
        self.scope_stack = []

    def report_name_reference(self,name):
        self.scope_stack[-1].add_name(name,Name.REFERENCED)
    def report_name_declaration(self,name):
        self.scope_stack[-1].add_name(name,Name.DECLARED)
    def push_scope(self):
        self.scope_stack.append(Scope())
    def pop_scope(self):
        return self.scope_stack.pop()
    def add_subscope(self,subscope):
        self.scope_stack[-1].add_subscope(subscope)
    def build_name_ids(self):
        global_scope = self.scope_stack[0]
        global_dict = {}

        def get_global_id(name):
            if name not in global_dict:
                global_dict[name] = len(global_dict)
            return global_dict[name]

        for name in global_scope.names.values():
            name.global_id = get_global_id(name.name)
            name.local_id = name.global_id
        def traverse_local_scope(scope,lookup):
            lookup = copy(lookup)
            def get_local_id(name):
                if name not in lookup:
                    lookup[name] = len(lookup)
                return lookup[name]
            for name in scope.names.values():
                name.global_id = get_global_id(name.name)
                name.local_id = get_local_id(name.name)
            for subscope in scope.subscopes:
                traverse_local_scope(subscope,lookup)
        for subscope in global_scope.subscopes: traverse_local_scope(subscope,{})
    # Set the "closed" flag for each name that is declared in a scope
    # and later referenced from a nested function.
    def mark_closed_names(self):
        global_scope = self.scope_stack[0]

        # Traverse the entire scope tree.
        def traverse(scope,declared):
            declared = copy(declared)

            for name in scope.names.values():
                if name.name in declared:
                    declared[name.name].closed = True


            for name in scope.names.values():
                if name.type == Name.DECLARED:
                    declared[name.name] = name
            for subscope in scope.subscopes:
                traverse(subscope,declared)

        for local_scope in global_scope.subscopes:
            traverse(local_scope,{})
        pass

    def next(self):
        self.discarded.append(self.current)
        self.current = self.tokens.popleft()
    def peek(self):
        return self.tokens[0]
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
            raise ParseError("Expected "+token_type+" but found "+self.current.type+" with value "+str(self.current.value),self.current.source)
        if value != None and self.current.value != value:
            raise ParseError("Expected: "+token_type+" with value: "+str(value),self.current.source)
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
            raise ParseError("Expected "+rule_class.__name__+" but found "+str(self.peek().type),self.peek().source)
        return self.matched_rule
    # Expect token of newline type.
    def expectNewline(self):
        self.expect(Token.NEWLINE)
    def acceptNewline(self):
        self.accept(Token.NEWLINE)
    # Check if next token is EOF without consuming token.
    def isNextEOF(self):
        self.next()
        result = self.current.type == Token.EOF
        self.back()
        return result

    def parse_program(self):
        program = self.expectRule(Program)
        self.build_name_ids()
        self.mark_closed_names()
        program.scope = self.pop_scope()
        return program
        

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
    NUMBER_TERMINAL = "rule_number_terminal"
    SELECT = "rule_select"
    SELECT_CASE = "rule_select_case"
    ANONYMOUS_FUNCTION = "rule_anonymous_function"
    INLINE_MAP = "rule_inline_map"
    KEY_VALUE = "rule_key_value"
    THROW = "rule_throw"
    TRY = "rule_try"
    CATCH = "rule_catch"
    FINALLY = "rule_finally"


    def __init__(self,nodes):
        self.nodes = nodes
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.nodes)
    
class Exit(Rule):
    type = Rule.EXIT
    NODE_EXPRESSION = "expression"
    @classmethod
    def match(cls,parser):
        if parser.accept(Token.KEYWORD,KeywordToken.EXIT):
            if parser.acceptRule(Expression):
                return Exit({Exit.NODE_EXPRESSION:parser.matched_rule})
            return Exit({})
        return None

class ExitLoop(Rule):
    type = Rule.EXITLOOP
    NODE_LEVEL = "level"
    @classmethod
    def match(cls,parser):
        if parser.accept(Token.KEYWORD,KeywordToken.EXITLOOP):
            nodes = {}
            if parser.accept(Token.INTEGER):
                nodes[ExitLoop.NODE_LEVEL] = parser.current
            return ExitLoop(nodes)
        return None
    
class ContinueLoop(Rule):
    type = Rule.CONTINUELOOP
    NODE_LEVEL = "level"
    @classmethod
    def match(cls,parser):
        if parser.accept(Token.KEYWORD,KeywordToken.CONTINUELOOP):
            nodes = {}
            if parser.accept(Token.INTEGER):
                nodes[ContinueLoop.NODE_LEVEL] = parser.current
            return ContinueLoop(nodes)
        return None

class DeclarationAssignment(Rule):
    type = Rule.ASSIGNMENT
    NODE_IDENTIFIER = "identifier"
    NODE_VALUE_EXPRESSION = "value expression"
    NODE_SUBSCRIPTS = "subscripts"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.IDENTIFIER):
            return None
        nodes = {DeclarationAssignment.NODE_IDENTIFIER: parser.current}
        parser.report_name_declaration(parser.current.value)

        subscripts = []
        while parser.acceptRule(ListIndexing):
            subscripts.append(parser.matched_rule)
        if len(subscripts)>0:
            nodes[DeclarationAssignment.NODE_SUBSCRIPTS] = subscripts

        if parser.accept(Token.OPERATOR,OperatorToken.EQUAL):
            nodes[DeclarationAssignment.NODE_VALUE_EXPRESSION] = parser.expectRule(Expression)
        return DeclarationAssignment(nodes)


class Declaration(Rule):
    type = Rule.DECLARATION
    NODE_SCOPE = "scope"
    NODE_ENUM = "enum"
    NODE_CONST = "const"
    NODE_VARIABLES = "variables"
    @classmethod
    def match(cls,parser): 
        if (parser.accept(Token.KEYWORD,KeywordToken.DIM) or 
                parser.accept(Token.KEYWORD,KeywordToken.LOCAL) or
                parser.accept(Token.KEYWORD,KeywordToken.GLOBAL)):
            nodes = {Declaration.NODE_SCOPE:parser.current}
            if parser.accept(Token.KEYWORD,KeywordToken.CONST):
                nodes[Declaration.NODE_CONST] = parser.current    
        elif parser.accept(Token.KEYWORD,KeywordToken.CONST):
            nodes = {Declaration.NODE_SCOPE:KeywordToken(KeywordToken.DIM)}
            nodes[Declaration.NODE_CONST] = parser.current 
        else:
            return None 

        if parser.acceptRule(Enum):
            nodes[Declaration.NODE_ENUM] = parser.matched_rule
            return Declaration(nodes)

        
        variables = []
        variables.append(parser.expectRule(DeclarationAssignment))
        
        while parser.accept(Token.COMMA):
            variables.append(parser.expectRule(DeclarationAssignment))
        nodes[Declaration.NODE_VARIABLES] = variables
        
        return Declaration(nodes)


class Enum(Rule):
    type = Rule.ENUM
    NODE_STEP = "step"
    NODE_ENUM_LIST = "enum list"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.ENUM):
            return None
        nodes = {}
        if parser.acceptRule(EnumStep):
            nodes[Enum.NODE_STEP] = parser.matched_rule
        nodes[Enum.NODE_ENUM_LIST] = parser.expectRule(EnumList)
        return Enum(nodes)

class EnumList(Rule):
    type = Rule.ENUMLIST
    NODE_CONSTANTS = "constants"
    @classmethod
    def match(cls,parser):
        if not parser.acceptRule(EnumConstant):
            return None
        nodes = [parser.matched_rule]
        while parser.accept(Token.COMMA):
            nodes.append(parser.expectRule(EnumConstant))
        return EnumList({EnumList.NODE_CONSTANTS:nodes})


class EnumConstant(Rule):
    type = Rule.ENUM_CONSTANT
    NODE_IDENTIFIER = "identifier"
    NODE_VALUE = "value"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.IDENTIFIER):
            return None
        nodes = {EnumConstant.NODE_IDENTIFIER:parser.current}
        parser.report_name_declaration(parser.current.value)
        if parser.accept(Token.OPERATOR,OperatorToken.EQUAL):
            nodes[EnumConstant.NODE_VALUE]=parser.expect(Token.INTEGER)
        return EnumConstant(nodes)

        
        
class EnumStep(Rule):
    type = Rule.ENUM_STEP
    NODE_OPERATOR = "operator"
    NODE_VALUE = "step"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.STEP):
            return None
        nodes = {}
        if parser.accept(Token.OPERATOR,OperatorToken.ADD) or parser.accept(Token.OPERATOR,OperatorToken.SUBTRACT) or parser.accept(Token.OPERATOR,OperatorToken.MULTIPLY):
            nodes[EnumStep.NODE_OPERATOR]=parser.current
        nodes[EnumStep.NODE_VALUE]=parser.expect(Token.INTEGER)
        return EnumStep(nodes)
            

class Assignment(Rule):
    type = Rule.ASSIGNMENT
    NODE_ASSIGNMENT_OPERATOR = "assignment operator"
    NODE_VALUE_EXPRESSION = "value expression"
    @classmethod
    def match(cls,parser):
        for operator in OperatorToken.ASSIGNMENT_OPERATORS:
            if parser.accept(Token.OPERATOR,operator):
                nodes = {Assignment.NODE_ASSIGNMENT_OPERATOR:parser.current}
                nodes[Assignment.NODE_VALUE_EXPRESSION]=parser.expectRule(Expression)
                return Assignment(nodes)
        return None


 
 
class ReDim(Rule):
    type = Rule.REDIM
    NODE_NAME = "name"
    NODE_QUALIFIERS = "qualifiers"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.REDIM):
            return None
        nodes = {ReDim.NODE_NAME:parser.expect(Token.IDENTIFIER)}
        parser.report_name_reference(parser.current.value)
        qualifiers = []
        while parser.acceptAnyRule([ListIndexing]):
            qualifiers.append(parser.matched_rule)
        nodes[ReDim.NODE_QUALIFIERS] = qualifiers
        return ReDim(nodes)
     
 
class LineStatement(Rule):
    type = Rule.LINE_STATEMENT
    NODE_START = "start"
    NODE_QUALIFIERS = "qualifiers"
    NODE_ASSIGNMENT = "assignment"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.IDENTIFIER):
            return None
        nodes = {LineStatement.NODE_START:parser.current}
        parser.report_name_reference(parser.current.value)
        
        qualifiers = []
        while parser.acceptRule(Qualifier):
            qualifiers.append(parser.matched_rule)
        nodes[LineStatement.NODE_QUALIFIERS] = qualifiers
            
        if parser.acceptRule(Assignment):
            parser.report_name_declaration(nodes[LineStatement.NODE_START].value)
            nodes[LineStatement.NODE_ASSIGNMENT] = parser.matched_rule
        return LineStatement(nodes)

class Select(Rule):
    type = Rule.SELECT
    NODE_CASE_LIST = "case_list"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.SELECT):
            return None
        parser.expect(Token.NEWLINE)
        case_list = []
        case_list += [parser.expectRule(SelectCase)]
        while parser.acceptRule(SelectCase):
            case_list += [parser.matched_rule]
        parser.expect(Token.KEYWORD,KeywordToken.ENDSELECT)
        return Select({Select.NODE_CASE_LIST:case_list})

class SelectCase(Rule):
    type = Rule.SELECT_CASE
    NODE_CONDITION = "condition"
    NODE_BODY = "body"
    NODE_ELSE = "else"
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.CASE):
            return None
        nodes = {}
        if parser.accept(Token.KEYWORD,KeywordToken.ELSE):
            nodes[SelectCase.NODE_ELSE] = parser.current
        else:
            nodes[SelectCase.NODE_CONDITION] = parser.expectRule(Expression)
        parser.expect(Token.NEWLINE)
        nodes[SelectCase.NODE_BODY] = parser.expectRule(Block)
        return SelectCase(nodes)
        

class Throw(Rule):
    type = Rule.THROW
    NODE_EXPRESSION = "expression"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.THROW):
            return None
        nodes = {Throw.NODE_EXPRESSION: parser.expectRule(Expression)}
        return Throw(nodes)

class Try(Rule):
    type = Rule.TRY
    NODE_BODY = "body"
    NODE_CATCH = "catch"
    NODE_FINALLY = "finally"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.TRY):
            return None
        nodes = {}
        nodes[Try.NODE_BODY] = parser.expectRule(Block)

        catch_node = parser.acceptRule(Catch)
        finally_node = parser.acceptRule(Finally)

        if not catch_node and not finally_node:
            raise ParseError("No catch or finally block found in try block",parser.current.source)

        if catch_node:
            nodes[Try.NODE_CATCH] = catch_node
        if finally_node:
            nodes[Try.NODE_FINALLY] = finally_node

        parser.expect(Token.KEYWORD,KeywordToken.ENDTRY)

        return Try(nodes)

class Catch(Rule):
    type = Rule.CATCH
    NODE_VARIABLE = "variable"
    NODE_BODY = "body"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.CATCH):
            return None
        nodes = {}

        nodes[Catch.NODE_VARIABLE] = parser.expect(Token.IDENTIFIER)
        if parser.acceptRule(Block):
            nodes[Catch.NODE_BODY] = parser.matched_rule
        parser.report_name_declaration(nodes[Catch.NODE_VARIABLE].value)
        return Catch(nodes)

class Finally(Rule):
    type = Rule.FINALLY
    NODE_BODY = "body"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.FINALLY):
            return None
        return Finally({Finally.NODE_BODY: parser.expectRule(Block)})

class Statement(Rule):
    type = Rule.STATEMENT
    NODE_SUBSTATEMENT = "substatement"
    @classmethod
    def match(cls,parser):
        if parser.acceptAnyRule([With,ReDim,Enum,Return,DoUntil,
                                 For,Directive,Exit,ExitLoop,ContinueLoop,
                                 Declaration,Function,While,If,Switch,Select,
                                 LineStatement,Throw, Try]):
            return Statement({Statement.NODE_SUBSTATEMENT:parser.matched_rule})
        #if parser.acceptRule(Expression):
        #    return Statement([parser.matched_rule])
        
        return None
    
    
class With(Rule):
    type = Rule.WITH
    NODE_OBJECT_EXPRESSION = "object_expression"
    NODE_STATEMENTS = "statements"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.WITH):
            return None
        nodes = {With.NODE_OBJECT_EXPRESSION: parser.expectRule(Expression)}
        parser.expect(Token.NEWLINE)
        
        statements = []
        while parser.accept(Token.DOT):
            statements.append(parser.expectRule(LineStatement))
            parser.expect(Token.NEWLINE)
        nodes[With.NODE_STATEMENTS] = statements
        parser.expect(Token.KEYWORD,KeywordToken.ENDWITH)
        return With(nodes)
            

class Return(Rule):
    type = Rule.RETURN
    NODE_EXPRESSION = "expression"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.RETURN):
            return None
        nodes = {}
        if parser.acceptRule(Expression):
            nodes[Return.NODE_EXPRESSION] = parser.matched_rule
        return Return(nodes)  
    
class DoUntil(Rule):
    type = Rule.DO_UNTIL
    NODE_BODY = "body"
    NODE_CONDITION = "condition"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.DO):
            return None
        parser.expect(Token.NEWLINE)
        nodes = {DoUntil.NODE_BODY: parser.expectRule(Block)}
        parser.expect(Token.KEYWORD,KeywordToken.UNTIL)
        nodes[DoUntil.NODE_CONDITION]=parser.expectRule(Expression)
        return DoUntil(nodes)    

class For(Rule):
    type = Rule.FOR
    NODE_LOOP_VARIABLE = "loop variable"
    NODE_FOR_TO = "for to"
    NODE_FOR_IN = "for in"
    NODE_BODY = "body"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.FOR):
            return None
        nodes = {For.NODE_LOOP_VARIABLE: parser.expect(Token.IDENTIFIER)}
        parser.report_name_declaration(parser.current.value)
        if parser.acceptRule(ForTo):
            nodes[For.NODE_FOR_TO]=parser.matched_rule
        else:
            nodes[For.NODE_FOR_IN]=parser.expectRule(ForIn)
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes[For.NODE_BODY]=parser.matched_rule
        parser.expect(Token.KEYWORD,KeywordToken.NEXT)
        return For(nodes)
            
class NumberTerminal(Rule):
    type = Rule.NUMBER_TERMINAL
    NODE_NUMBER = "terminal"
    NODE_NEGATIVE = "negative"
    @classmethod
    def match(cls,parser):
        if parser.accept(Token.OPERATOR,OperatorToken.SUBTRACT):
            return NumberTerminal({NumberTerminal.NODE_NEGATIVE:True,
                                   NumberTerminal.NODE_NUMBER: parser.expect(Token.INTEGER)})
        if parser.accept(Token.INTEGER):
            return NumberTerminal({NumberTerminal.NODE_NUMBER:parser.current})

class ForTo(Rule):
    type = Rule.FOR_TO
    NODE_INIT_EXPRESSION = "init expression"
    NODE_END_EXPRESSION = "end expression"
    NODE_STEP_VALUE = "step value"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.OPERATOR,OperatorToken.EQUAL):
            return None
        nodes = {ForTo.NODE_INIT_EXPRESSION: parser.expectRule(Expression)}
        parser.expect(Token.KEYWORD,KeywordToken.TO)
        nodes[ForTo.NODE_END_EXPRESSION]=parser.expectRule(Expression)
        if parser.accept(Token.KEYWORD,KeywordToken.STEP):
            nodes[ForTo.NODE_STEP_VALUE]=parser.expectRule(NumberTerminal)
        return ForTo(nodes)    
        
        
        
class ForIn(Rule):
    type = Rule.FOR_IN
    NODE_LIST_EXPRESSION = "list expression"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.IN):
            return None
        return ForIn({ForIn.NODE_LIST_EXPRESSION:parser.expectRule(Expression)})       


class Qualifier(Rule):
    type = Rule.QUALIFIER
    NODE_SUBQUALIFIER = "subqualifier"
    @classmethod
    def match(cls,parser):
        if parser.acceptAnyRule([Property,Call,ListIndexing]):
            return Qualifier({Qualifier.NODE_SUBQUALIFIER: parser.matched_rule})
        return None    
            
class ArgumentList(Rule):
    type = Rule.ARGUMENT_LIST
    NODE_ARGUMENT_LIST = "argument list"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.LEFT_PAREN):
            return None
    
        nodes = {}
        
        arguments = []
        if parser.acceptRule(Argument):
            arguments.append(parser.matched_rule)
            while parser.accept(Token.COMMA):
                arguments.append(parser.acceptRule(Argument))     
        nodes[ArgumentList.NODE_ARGUMENT_LIST] = arguments 
        
        parser.expect(Token.RIGHT_PAREN) 
        return ArgumentList(nodes)   
    
class Argument(Rule):
    type = Rule.ARGUMENT
    NODE_BYREF = "byref"
    NODE_NAME = "name"
    NODE_DEFAULT_VALUE = "default value"
    NODE_CONST = "const"
    @classmethod
    def match(cls,parser):
        nodes = {}

        if parser.accept(Token.KEYWORD,KeywordToken.CONST):
            nodes[Argument.NODE_CONST] = parser.current


        if parser.accept(Token.KEYWORD,KeywordToken.BYREF):
            nodes[Argument.NODE_BYREF] = parser.current
        
        if len(nodes) > 0:
            nodes[Argument.NODE_NAME] = parser.expect(Token.IDENTIFIER)
        elif parser.accept(Token.IDENTIFIER):
            nodes[Argument.NODE_NAME] = parser.current
        else: 
            return None
        parser.report_name_declaration(nodes[Argument.NODE_NAME].value)
        if parser.accept(Token.OPERATOR,OperatorToken.EQUAL):
            nodes[Argument.NODE_DEFAULT_VALUE] = parser.expectRule(Expression)
        return Argument(nodes)
            

class Function(Rule):
    type = Rule.FUNCTION
    NODE_NAME = "name"
    NODE_ARGUMENTS = "arguments list"
    NODE_BODY = "body"

    def __init__(self, nodes,scope):
        super().__init__(nodes)
        self.scope = scope

    @classmethod
    def match(cls,parser):
        if parser.accept(Token.KEYWORD,KeywordToken.FUNC):

            

            nodes = {}
            nodes[Function.NODE_NAME]=parser.expect(Token.IDENTIFIER)
            parser.report_name_declaration(parser.current.value)
            parser.push_scope()
            nodes[Function.NODE_ARGUMENTS]=parser.expectRule(ArgumentList)
            parser.expectNewline() 
            
            if parser.acceptRule(Block):
                nodes[Function.NODE_BODY]=parser.matched_rule
            
            parser.expect(Token.KEYWORD,KeywordToken.ENDFUNC)

            my_scope = parser.pop_scope()
            parser.add_subscope(my_scope)
            
            return Function(nodes,my_scope)
        else:
            return None

class AnonymousFunction(Function):
    type = Rule.ANONYMOUS_FUNCTION

    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.FUNC):
            return None
        parser.push_scope()
        nodes = {}
        nodes[Function.NODE_ARGUMENTS] = parser.expectRule(ArgumentList)
        nodes[Function.NODE_BODY] = parser.acceptRule(InlineBlock)
        parser.expect(Token.KEYWORD,KeywordToken.ENDFUNC)
        my_scope = parser.pop_scope()
        parser.add_subscope(my_scope)
        return AnonymousFunction(nodes,my_scope)
        
class Block(Rule):
    type = Rule.BLOCK
    NODE_STATEMENTS = "statements"
    require_newline = True
    @classmethod
    def match(cls,parser):
        nodes = {Block.NODE_STATEMENTS:[]}
        something_matched = False
        while True:
            if parser.accept(Token.NEWLINE):
                something_matched = True
                continue
            if parser.acceptRule(Statement):
                something_matched = True
                nodes[Block.NODE_STATEMENTS].append(parser.matched_rule)
                if not parser.isNextEOF():
                    if cls.require_newline:
                        parser.expect(Token.NEWLINE)
                    else:
                        parser.accept(Token.NEWLINE)
                continue
            break
        if something_matched: return Block(nodes)
        return None

class InlineBlock(Block):
    require_newline = False

class While(Rule):
    
    NODE_CONDITION = "condition"
    NODE_BODY = "body"
    
    type = Rule.WHILE
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.WHILE):
            return None
        nodes = {}
        nodes[While.NODE_CONDITION]=parser.expectRule(Expression)
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes[While.NODE_BODY]=parser.matched_rule
        parser.expect(Token.KEYWORD,KeywordToken.WEND)
        return While(nodes)


class Switch(Rule):
    type = Rule.SWITCH
    NODE_VALUE = "value"
    NODE_CASES = "cases"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.SWITCH):
            return None
        nodes = {}
        nodes[Switch.NODE_VALUE]=parser.expectRule(Expression)
        parser.expect(Token.NEWLINE)
        
        cases = []
        while parser.acceptRule(SwitchCase):
            cases.append(parser.matched_rule)
        nodes[Switch.NODE_CASES] = cases 
            
        parser.expect(Token.KEYWORD,KeywordToken.ENDSWITCH)
        return Switch(nodes)
    
    
class SwitchCondition(Rule):
    type = Rule.SWITCH_CONDITION
    NODE_FROM = "from"
    NODE_TO = "to"
    NODE_ELSE = "else"
    @classmethod
    def match(cls,parser):

        if parser.accept(Token.KEYWORD,KeywordToken.ELSE):
            return SwitchCondition({SwitchCondition.NODE_ELSE:parser.current})

        if not parser.acceptRule(Expression):
            return None
        nodes = {SwitchCondition.NODE_FROM: parser.matched_rule}
        if parser.accept(Token.KEYWORD,KeywordToken.TO):
            nodes[SwitchCondition.NODE_TO]=parser.expectRule(Expression)
        return SwitchCondition(nodes)
    
class SwitchCase(Rule):  
    type = Rule.SWITCH_CASE  
    NODE_CONDITIONS = "conditions"
    NODE_BODY = "body"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.CASE):
            return None
        
        
        conditions = [parser.expectRule(SwitchCondition)]
        while parser.accept(Token.COMMA):
            conditions.append(parser.expectRule(SwitchCondition))
        nodes = {SwitchCase.NODE_CONDITIONS: conditions}
        
        parser.expect(Token.NEWLINE)
        
        nodes[SwitchCase.NODE_BODY] = parser.expectRule(Block)
        
        return SwitchCase(nodes)
            
        
        
        
class If(Rule):
    type = Rule.IF
    NODE_CONDITION = "condition"
    NODE_BODY = "body"
    NODE_ELSEIFS = "elseifs"
    NODE_ELSE = "else"
    NODE_INLINE_STATEMENT = "inline statement"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.IF):
            return None
        nodes = {If.NODE_CONDITION: parser.expectRule(Expression)}
        parser.expect(Token.KEYWORD,KeywordToken.THEN)
        if parser.accept(Token.NEWLINE):
            if parser.acceptRule(Block):
                nodes[If.NODE_BODY]=parser.matched_rule
            elseifs = []
            while parser.acceptRule(ElseIf):
                elseifs.append(parser.matched_rule)
            nodes[If.NODE_ELSEIFS] = elseifs
            if parser.acceptRule(Else):
                nodes[If.NODE_ELSE] = parser.matched_rule
            parser.expect(Token.KEYWORD,KeywordToken.ENDIF)
        else:
            nodes[If.NODE_INLINE_STATEMENT] = parser.expectRule(Statement)
        return If(nodes)

class ElseIf(Rule):
    type = Rule.ELSEIF
    NODE_CONDITION = "condition"
    NODE_BODY = "body"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.ELSEIF):
            return None 
        nodes = {ElseIf.NODE_CONDITION:parser.expectRule(Expression)}
        parser.expect(Token.KEYWORD,KeywordToken.THEN)
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes[ElseIf.NODE_BODY]=parser.matched_rule
        return ElseIf(nodes)

class Else(Rule):
    type = Rule.ELSE
    NODE_BODY = "body"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.KEYWORD,KeywordToken.ELSE):
            return None 
        nodes = {}
        parser.expect(Token.NEWLINE)
        if parser.acceptRule(Block):
            nodes[Else.NODE_BODY]=parser.matched_rule
        return Else(nodes)

class Directive(Rule):
    type = Rule.DIRECTIVE
    NODE_TYPE = "type"
    NODE_ARGUMENT = "arg"
    @classmethod
    def match(cls,parser):
        argument = None
        directive = None
        
        if parser.accept(Token.DIRECTIVE, "#ONAUTOITSSTARTREGISTER"):
            directive = parser.current
            argument = parser.expect(Token.STRING)
            parser.expectNewline()
            return Directive({Directive.NODE_TYPE:directive,Directive.NODE_ARGUMENT:argument})
        if parser.accept(Token.DIRECTIVE):
            token = parser.current
            parser.skip_to_newline()
            return Directive({Directive.NODE_TYPE:token})
        
        return None        
        
class UnaryOperator(Rule):
    type = Rule.UNARY_OPERATOR
    NODE_OPERATOR = "operator"
    @classmethod
    def match(cls,parser):
        for operator in OperatorToken.UNARY_OPERATORS:  
            if parser.accept(Token.OPERATOR,operator):
                return UnaryOperator({UnaryOperator.NODE_OPERATOR:parser.current})


class Factor(Rule):
    type = Rule.FACTOR
    NODE_UNARIES = "unaries"
    NODE_SUBNODE = "subnode"
    NODE_QUALIFIERS = "qualifiers"
    @classmethod
    def match(cls,parser):
        nodes = {}
        
        unaries = []
        while parser.acceptRule(UnaryOperator):
            unaries.append(parser.matched_rule)
        nodes[Factor.NODE_UNARIES] = unaries
        
        
        if parser.acceptAnyRule([Terminal,InlineList,InlineMap]):
            nodes[Factor.NODE_SUBNODE]=parser.matched_rule
        elif parser.accept(Token.LEFT_PAREN):
            nodes[Factor.NODE_SUBNODE]=parser.expectRule(Expression)
            parser.expect(Token.RIGHT_PAREN)
        else:
            return None
        qualifiers = []
        while parser.acceptRule(Qualifier):
            qualifiers.append(parser.matched_rule)
        nodes[Factor.NODE_QUALIFIERS] = qualifiers    
            
        return Factor(nodes)

                
                
        
class InlineList(Rule):
    type = Rule.INLINE_LIST
    NODE_ELEMENTS = "elements"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.LEFT_BRACKET):
            return None
        nodes = {}
        expressions = []
        parser.acceptNewline()
        if parser.acceptRule(Expression):
            expressions.append(parser.matched_rule)
            while parser.accept(Token.COMMA):
                parser.acceptNewline()
                expressions.append(parser.expectRule(Expression))
        nodes[InlineList.NODE_ELEMENTS] = expressions
        parser.acceptNewline()
        parser.expect(Token.RIGHT_BRACKET)
        return InlineList(nodes)
        
            

class Call(Rule):
    type = Rule.CALL
    NODE_ARGUMENTS = "arguments"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.LEFT_PAREN):
            return None
        nodes = {}
        arguments = []
        if parser.acceptRule(Expression):
            arguments.append(parser.matched_rule)
            while parser.accept(Token.COMMA):
                arguments.append(parser.expectRule(Expression))
        nodes[Call.NODE_ARGUMENTS] = arguments
        parser.expect(Token.RIGHT_PAREN)
        return Call(nodes)
        

class Property(Rule):
    type = Rule.PROPERTY
    NODE_NAME = "name"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.DOT):
            return None
        return Property({Property.NODE_NAME:parser.expect(Token.IDENTIFIER)})


class ListIndexing(Rule):
    type = Rule.LIST_INDEXING
    NODE_INDEX = "index"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.LEFT_BRACKET):
            return None
        nodes = {ListIndexing.NODE_INDEX:parser.expectRule(Expression)}
        parser.expect(Token.RIGHT_BRACKET)
        return ListIndexing(nodes)

class Terminal(Rule):
    type = Rule.TERMINAL
    NODE_TYPE = "type"
    @classmethod
    def match(cls,parser):
        if parser.accept(Token.IDENTIFIER):
            parser.report_name_reference(parser.current.value)
            return Terminal({Terminal.NODE_TYPE:parser.current})
        accepted = [Token.BOOLEAN,Token.MACRO,Token.INTEGER,Token.STRING,Token.FLOATING]
        for token_type in accepted:
            if parser.accept(token_type):
                return Terminal({Terminal.NODE_TYPE:parser.current})
        if parser.accept(Token.KEYWORD,KeywordToken.DEFAULT):
            return Terminal({Terminal.NODE_TYPE:parser.current})
        if parser.acceptRule(AnonymousFunction):
            return Terminal({Terminal.NODE_TYPE:parser.matched_rule})
        return None


class BinaryOperator(Rule):
    
    type = Rule.BINARY_OPERATOR
    NODE_OPERATOR = "operator"
    
    PRECEDENCE_LEVELS = [[OperatorToken.BOOLEAN_AND,OperatorToken.BOOLEAN_OR],
              [OperatorToken.GREATER,OperatorToken.LESSER,OperatorToken.GREATER_EQUAL,
                                    OperatorToken.LESSER_EQUAL,OperatorToken.EQUAL,
                                    OperatorToken.NOT_EQUAL,OperatorToken.STRONG_STRING_EQUAL,OperatorToken.EXACTLY_EQUAL],
              [OperatorToken.CONCAT],
              [OperatorToken.ADD,OperatorToken.SUBTRACT],
              [OperatorToken.MULTIPLY,OperatorToken.DIVIDE],
              [OperatorToken.POW]
              ]
    @classmethod
    def match(cls,parser):
        for operator in OperatorToken.BINARY_OPERATORS:
            if parser.accept(Token.OPERATOR,operator):
                return BinaryOperator({BinaryOperator.NODE_OPERATOR:parser.current})
        return None
        
    def precedence_level(self):
        for index,level in enumerate(BinaryOperator.PRECEDENCE_LEVELS):
            if self.nodes[BinaryOperator.NODE_OPERATOR].value in level:
                return index
        raise Exception("No precedence level for token "+str(self.value))
    def is_left_associative(self):
        return self.nodes[BinaryOperator.NODE_OPERATOR].value != OperatorToken.POW


class InlineMap(Rule):
    type = Rule.INLINE_MAP
    NODE_KEY_VALUES = "key values"
    @classmethod
    def match(cls,parser):
        if not parser.accept(Token.LEFT_CURLY_BRACKET):
            return None
        key_values = []
        parser.acceptNewline()
        if parser.acceptRule(KeyValue):
            key_values += [parser.matched_rule]
            while parser.accept(Token.COMMA):
                parser.acceptNewline()
                key_values += [parser.expectRule(KeyValue)]
        parser.acceptNewline()
        parser.expect(Token.RIGHT_CURLY_BRACKET)
        return InlineMap({InlineMap.NODE_KEY_VALUES: key_values})

class KeyValue(Rule):
    type = Rule.KEY_VALUE
    NODE_KEY = "key"
    NODE_VALUE = "value"
    @classmethod
    def match(cls,parser):
        if not parser.acceptRule(Expression):
            return None
        key = parser.matched_rule
        parser.expect(Token.COLON)
        parser.acceptNewline()
        value = parser.expectRule(Expression)
        return KeyValue({KeyValue.NODE_KEY: key, KeyValue.NODE_VALUE: value})
    
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
    
    @classmethod
    def climb_precedence(cls,nodes):
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
    
    @classmethod
    def match(cls,parser):
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
    NODE_BLOCK = "block"
    @classmethod
    def match(cls,parser):
        nodes = {}
        # * Global scope *
        parser.push_scope()
        if parser.acceptRule(Block):
            nodes[Program.NODE_BLOCK] = parser.matched_rule
        parser.expect(Token.EOF)
        return Program(nodes)
    
    
def print_ast(node,depth=0):
    
    if isinstance(node,list):
        for elem in node:
            print_ast(elem,depth+1)
        return
    if isinstance(node,dict):
        for key in node:
            print_ast(node[key],depth+1)
        return
    
    print(" "*depth,node.__class__.__name__,end="")
    if "nodes" in node.__dict__:
        print("")
        for key in node.nodes:
            print_ast(node.nodes[key],depth+1)
    else:
        print(" "+str(node.value))
        
   


#test_code = """if 1+2==3 then a = 10"""
#test_code = open("test.au3").read()
#tokens = lexer.lex_string(test_code)
#parser = Parser(tokens)
#p = parser.acceptRule(Program)
#print_ast(p)    
    