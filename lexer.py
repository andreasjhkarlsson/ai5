
import re

def lex_string(string):
    
    token_classes = [IncludeFileToken,CommaToken,LeftParenToken,RightParenToken,LeftBracketToken,RightBracketToken, KeywordToken,
                     FloatingToken,DotToken,MacroToken,CommentToken,DirectiveToken,NewlineToken,WhitespaceToken,
                     OperatorToken,IntegerToken,StringToken,BooleanToken,IdentifierToken]
    tokens = []
    
    offset = 0
    
    inc = 0
    while len(string) > offset:
        inc += 1
        if (inc % 10000) == 0:
            print(offset)
        for token_class in token_classes:
            result = token_class.match(string,offset)
            if result:
                new_token = result[0]
                offset += result[1]
                if new_token.type != Token.WHITESPACE and new_token.type != Token.COMMENT:
                    tokens.append(new_token)
                break
        else:
            raise Exception("Could not scan string: "+string[offset:])
    tokens.append(EOFToken())    
    
    return tokens

#Match a regex on string starting at offset.
# Returns the match as a string, if any or None.
def simple_regex_search(string,compiled_expression,offset):
    m = compiled_expression.match(string,offset)
    if m:
        return m.group(0)
    return None


class Token(object):
    INTEGER = "integer"
    OPERATOR = "operator"
    STRING = "string"
    WHITESPACE = "whitespace"
    NEWLINE = "newline"
    IDENTIFIER = "identifier"
    MACRO = "macro"
    COMMENT = "comment"
    DIRECTIVE = "directive"
    KEYWORD = "keyword"
    BOOLEAN = "boolean"
    FLOATING = "floating"
    LEFT_PAREN = "left parenthesis"
    RIGHT_PAREN = "right parenthesis"
    LEFT_BRACKET = "left bracket"
    RIGHT_BRACKET = "right bracket"
    DOT = "dot"
    COMMA = "comma"
    EOF = "eof"
    INCLUDE_FILE = "include file"
    
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
        
    
    def __init__(self,value=None):
        self.value = value
        
    @classmethod
    def value_transform(cls,value):
        return value
        
    @classmethod
    def match(cls,string,offset=0):
        res = simple_regex_search(string, cls.expr,offset)
        if res:
            return cls(cls.value_transform(res)), len(res)
        return None        

class IncludeFileToken(Token):
    type = Token.INCLUDE_FILE
    expr = re.compile(r'''#(\w|\-)+\s+((<[^>]+>)|"[^"]+")''')
    @classmethod
    def value_transform(cls,value):
        quote_char = value[-1]
        if quote_char == ">":
            quote_char = "<"
        return value[value.find(quote_char)+1:-1]
    
class EOFToken(Token):
    type = Token.EOF

class CommaToken(Token):
    expr = re.compile(r''',''')
    type = Token.COMMA

class FloatingToken(Token):
    expr = re.compile(r'''\d*\.\d+|\d+\.\d*''')
    type = Token.FLOATING
    @classmethod
    def value_transform(cls,value):
        return float(value)

class BooleanToken(Token):
    expr = re.compile(r'''true|false''',re.IGNORECASE)
    type = Token.BOOLEAN
    @classmethod
    def value_transform(cls,value):
        return value.lower() == "true"



class MacroToken(Token):
    expr = re.compile(r'''@\w+''')
    type = Token.MACRO
    @classmethod
    def value_transform(cls,value):
        return value[1:].lower()
    
class CommentToken(Token):
    expr = re.compile(r''';.*|(#comments-start|#cs)(.|\s)*(#comments-end|#ce)''',re.IGNORECASE)
    type = Token.COMMENT

class DirectiveToken(Token):
    expr = re.compile(r'''#(\w|\-)+''')
    type = Token.DIRECTIVE
    @classmethod
    def value_transform(cls,value):
        return value[1:].lower()
    
   
class IdentifierToken(Token):
    expr = re.compile(r'''(\$\w+)|([A-Za-z_]{1}\w*)''')
    type = Token.IDENTIFIER
    @classmethod
    def value_transform(cls,value):
        if value[0] == "$":
            return value[1:].lower()
        return value.lower()

class NewlineToken(Token):
    expr = re.compile(r"""((\n+)|((\r\n)+))""")
    type = Token.NEWLINE

class IntegerToken(Token):
    expr = re.compile(r"""(0x[A-Fa-f0-9]+)|(\d+)""")
    type = Token.INTEGER
    @classmethod
    def value_transform(cls,value):
        return int(value.lower(),0)

class WhitespaceToken(Token):
    expr = re.compile(r"""([^\S\n\r]+)""")
    type = Token.WHITESPACE
    
class OperatorToken(Token):
    expr = re.compile(r""">\=|<\=|<>|<|>|\-\=|\*|\/\=|&\=|\+\=|\+|\-|\*|\/|&|\=\=|\=|\^|AND|OR|NOT""",re.IGNORECASE)
    type = Token.OPERATOR
    
    
class StringToken(Token):
    #expr = re.compile("""(?:^\"([^\"]*)\")|(?:^'([^']*)')""")
    expr = re.compile(r'''('(?:\\.|[^'])*')|("(?:\\.|[^"])*")''')
    type = Token.STRING
    @classmethod
    def value_transform(cls,value):
        return value[1:-1].replace(r'\"','"').replace(r"\'","'")   


class LeftParenToken(Token):
    expr = re.compile(r'''\(''')
    type = Token.LEFT_PAREN

class RightParenToken(Token):
    expr = re.compile(r'''\)''')
    type = Token.RIGHT_PAREN
    
class LeftBracketToken(Token):
    expr = re.compile(r'''\[''')
    type = Token.LEFT_BRACKET
    
class RightBracketToken(Token):
    expr = re.compile(r'''\]''')
    type = Token.RIGHT_BRACKET
    
class DotToken(Token):
    expr = re.compile(r'''\.''')
    type = Token.DOT
    
class KeywordToken(Token):
    CONTINUECASE = "CONTINUECASE".lower()
    CONTINUELOOP = "CONTINUELOOP".lower()
    DEFAULT = "DEFAULT".lower()
    DIM = "DIM".lower()
    GLOBAL = "GLOBAL".lower()
    LOCAL = "LOCAL".lower()
    CONST = "CONST".lower()
    DO = "DO".lower()
    UNTIL = "UNTIL".lower()
    ENUM = "ENUM".lower()
    EXIT = "EXIT".lower()
    EXITLOOP = "EXITLOOP".lower()
    FOR = "FOR".lower()
    TO = "TO".lower()
    STEP = "STEP".lower()
    NEXT = "NEXT".lower()
    IN = "IN".lower()
    FUNC = "FUNC".lower()
    RETURN = "RETURN".lower()
    ENDFUNC = "ENDFUNC".lower()
    IF = "IF".lower().lower()
    THEN = "THEN".lower()
    ELSE = "ELSE".lower()
    ELSEIF = "ELSEIF".lower()
    ENDIF = "ENDIF".lower()
    REDIM = "REDIM".lower()
    SELECT = "SELECT".lower()
    CASE = "CASE".lower()
    ENDSELECT = "ENDSELECT".lower()
    STATIC = "STATIC".lower()
    SWITCH = "SWITCH".lower()
    ENDSWITCH = "ENDSWITCH".lower()
    WHILE = "WHILE".lower()
    WEND = "WEND".lower()
    WITH = "WITH".lower()
    ENDWITH = "ENDWITH".lower()
    BYREF = "BYREF".lower()
    regex = ""
    keywords = [CONTINUECASE,CONTINUELOOP,DEFAULT,DIM,GLOBAL,LOCAL,CONST,
                    DO,UNTIL,ENUM,EXIT,EXITLOOP,FOR,TO,STEP,NEXT,IN,FUNC,
                    RETURN,ENDFUNC,IF,THEN,ELSE,ELSEIF,ENDIF,REDIM,
                    SELECT,CASE,ENDSELECT,STATIC,SWITCH,ENDSWITCH,WHILE,WEND,WITH,ENDWITH,BYREF]
    
    keywords.sort(key=lambda x:len(x), reverse=True)
    for keyword in keywords:
        if len(regex) > 0:
            regex += "|"
        regex += keyword
    
    
     
    expr = re.compile(regex,re.IGNORECASE)
    type = Token.KEYWORD

    @classmethod
    def value_transform(cls,value):
        return value.lower()


