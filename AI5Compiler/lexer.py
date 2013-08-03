import re
import os
import codecs

class Source:
    def __init__(self,filename,line_number):
        self.filename = filename
        self.line_number = line_number

class LexError(Exception):
    def __init__(self,message,source):
        self.message = message
        self.source = source


#Match a regex on string starting at offset.
# Returns the match as a string, if any or None.
def simple_regex_search(string,compiled_expression,offset):
    m = compiled_expression.match(string,offset)
    if m:
        return m.group(0)
    return None


class Token(object):
    INTEGER = "integer token"
    OPERATOR = "operator token"
    STRING = "string token"
    WHITESPACE = "whitespace token"
    NEWLINE = "newline token"
    IDENTIFIER = "identifier token"
    MACRO = "macro token"
    COMMENT = "comment token"
    DIRECTIVE = "directive token"
    KEYWORD = "keyword token"
    BOOLEAN = "boolean token"
    FLOATING = "floating token"
    LEFT_PAREN = "left parenthesis token"
    RIGHT_PAREN = "right parenthesis token"
    LEFT_BRACKET = "left bracket token"
    RIGHT_BRACKET = "right bracket token"
    LEFT_CURLY_BRACKET = "left curly bracket"
    RIGHT_CURLY_BRACKET = "right curly bracket"
    COLON = "colon"
    DOT = "dot token"
    COMMA = "comma token"
    EOF = "eof token"
    INCLUDE_FILE = "include file token"
    
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
        
    
    def __init__(self,value=None):
        self.value = value
        
    @classmethod
    def value_transform(cls,value):
        return value
    
    @classmethod
    def count_lines(cls,str):
        crlfs = str.count("\r\n")
        str = str.replace("\r\n","")
        lfs = str.count("\n")
        str = str.replace("\n","")
        crs = str.count("\r")   
        return crlfs+lfs+crs

    @classmethod
    def match(cls,string,offset=0):
        res = simple_regex_search(string, cls.expr,offset)
        if res:
            return cls(cls.value_transform(res)), len(res), cls.count_lines(res)
        return None        


class IncludeFile:
    SCRIPT_RELATIVE = "script relative"
    COMPILER_RELATIVE = "runtime relative"
    def __init__(self,type,path):
        self.type = type
        self.path = path

class IncludeFileToken(Token):
    type = Token.INCLUDE_FILE
    expr = re.compile(r'''#(\w|\-)+\s+((<[^>]+>)|"[^"]+")''')
    @classmethod
    def value_transform(cls,value):
        quote_char = value[-1]
        type = IncludeFile.SCRIPT_RELATIVE
        if quote_char == ">":
            quote_char = "<"
            type = IncludeFile.COMPILER_RELATIVE
        return IncludeFile(type,value[value.find(quote_char)+1:-1])
    
class EOFToken(Token):
    type = Token.EOF

class CommaToken(Token):
    expr = re.compile(r''',''')
    type = Token.COMMA

class ColonToken(Token):
    expr = re.compile(r''':''')
    type = Token.COLON
class LeftCurlyBracketToken(Token):
    expr = re.compile(r'''{''')
    type = Token.LEFT_CURLY_BRACKET
class RightCurlyBracketToken(Token):
    expr = re.compile(r'''}''')
    type = Token.RIGHT_CURLY_BRACKET

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
    expr = re.compile(r''';.*|(#comments-start|#cs)(.|\s)*?(#comments-end|#ce)''',re.IGNORECASE)
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
    expr = re.compile(r"""(0x[A-Fa-f0-9]+)|(\d+)""",re.IGNORECASE)
    type = Token.INTEGER
    @classmethod
    def value_transform(cls,value):
        return int(value.lower(),0)

class WhitespaceToken(Token):
    expr = re.compile(r"""([^\S\n\r]+)""")
    type = Token.WHITESPACE
    
class OperatorToken(Token):
    GREATER_EQUAL = ">="
    LESSER_EQUAL = "<="
    NOT_EQUAL = "<>"
    GREATER = ">"
    LESSER = "<"
    ADD_ASSIGN = "+="
    SUBTRACT_ASSIGN = "-="
    MULTIPLY_ASSIGN = "*="
    DIVIDE_ASSIGN = "/="
    CONCAT_ASSIGN = "&="
    ADD = "+"
    SUBTRACT = "-"
    MULTIPLY = "*"
    DIVIDE = "/"
    CONCAT = "&"
    STRONG_STRING_EQUAL = "=="
    EQUAL = "="
    POW = "^"
    BOOLEAN_AND = "and"
    BOOLEAN_OR = "or"
    BOOLEAN_NOT = "not"
    EXACTLY_EQUAL = "==="
    
    OPERATORS = [GREATER_EQUAL,LESSER_EQUAL,NOT_EQUAL,GREATER,LESSER,
                 ADD_ASSIGN,SUBTRACT_ASSIGN,MULTIPLY_ASSIGN,DIVIDE_ASSIGN,
                 CONCAT_ASSIGN,ADD,SUBTRACT,MULTIPLY,DIVIDE,CONCAT,
                 STRONG_STRING_EQUAL,EQUAL,POW,BOOLEAN_AND,BOOLEAN_NOT,BOOLEAN_OR,EXACTLY_EQUAL]
    
    BINARY_OPERATORS = [GREATER_EQUAL,LESSER_EQUAL,NOT_EQUAL,GREATER,LESSER,
                        ADD,SUBTRACT,MULTIPLY,DIVIDE,CONCAT,
                        STRONG_STRING_EQUAL,EQUAL,POW,BOOLEAN_AND,BOOLEAN_OR,EXACTLY_EQUAL]
    UNARY_OPERATORS = [BOOLEAN_NOT,SUBTRACT]
    ASSIGNMENT_OPERATORS = [EQUAL,ADD_ASSIGN,SUBTRACT_ASSIGN,MULTIPLY_ASSIGN,DIVIDE_ASSIGN,CONCAT_ASSIGN]
    
    regex = ""
    OPERATORS.sort(key=lambda x:len(x), reverse=True)
    for operator in OPERATORS:
        if len(regex) > 0:
            regex += "|"
        regex += re.escape(operator)
    
    expr = re.compile(regex,re.IGNORECASE)
    type = Token.OPERATOR
    @classmethod
    def value_transform(cls,value):
        return value.lower()
    
class StringToken(Token):
    #expr = re.compile("""(?:^\"([^\"]*)\")|(?:^'([^']*)')""")
    expr = re.compile(r'''("(?:[^"]|"")*")|('(?:[^']|'')*')''')
    type = Token.STRING
    @classmethod
    def value_transform(cls,value):
        if value[0]=='"':
            return value[1:-1].replace(r'""','"')
        else:
            return value[1:-1].replace(r"''","'") 


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
    IF = "IF".lower()
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
    TRY = "TRY".lower()
    CATCH = "CATCH".lower()
    FINALLY = "FINALLY".lower()
    ENDTRY = "ENDTRY".lower()
    THROW = "THROW".lower()
    regex = "("
    keywords = [CONTINUECASE,CONTINUELOOP,DEFAULT,DIM,GLOBAL,LOCAL,CONST,
                    DO,UNTIL,ENUM,EXIT,EXITLOOP,FOR,TO,STEP,NEXT,IN,FUNC,
                    RETURN,ENDFUNC,IF,THEN,ELSE,ELSEIF,ENDIF,REDIM,
                    SELECT,CASE,ENDSELECT,STATIC,SWITCH,ENDSWITCH,
                    WHILE,WEND,WITH,ENDWITH,BYREF,TRY,CATCH,FINALLY,
                    ENDTRY,THROW]
    
    keywords.sort(key=lambda x:len(x), reverse=True)
    for keyword in keywords:
        if len(regex) > 1:
            regex += "|"
        regex += keyword
    regex += ")(?=[^\w]|$)"
    
     
    expr = re.compile(regex,re.IGNORECASE)
    type = Token.KEYWORD

    @classmethod
    def value_transform(cls,value):
        return value.lower().rstrip()


class Lexer:
    TOKEN_CLASSES = [IncludeFileToken,CommaToken,LeftParenToken,RightParenToken,LeftBracketToken,RightBracketToken,
                    LeftCurlyBracketToken,RightCurlyBracketToken,ColonToken, KeywordToken,
                     FloatingToken,DotToken,MacroToken,CommentToken,DirectiveToken,NewlineToken,WhitespaceToken,
                     OperatorToken,IntegerToken,StringToken,BooleanToken,IdentifierToken]
    def __init__(self,compiler_include_directory):
        self.file_skip_filter = set()
        self.compiler_include_directory = compiler_include_directory

    def get_script_directory(self,filename):
        dir = os.path.dirname(filename)
        if dir == "":
            dir = os.getcwd()
        return dir+"\\"

    def get_included_file(self,source_file,include_file):
        if include_file.type == IncludeFile.COMPILER_RELATIVE:
            full_path = self.compiler_include_directory + include_file.path
        else:
            full_path = self.get_script_directory(source_file) + include_file.path
        include_file_handle = codecs.open(full_path,"r","utf-8")
        include_file_string = include_file_handle.read()
        include_file_handle.close()
        tokens = self.lex_string(include_file_string,full_path)  
        if len(tokens) > 0: tokens.pop(-1)   # Pop EOF token as this is just an include :)
        return tokens

    def lex_string(self,string,source_file):

        tokens = []

        # This file has been specified to be included only once (include-once)
        if source_file in self.file_skip_filter:
            return tokens
    
        offset = 0

        line_number = 1
 
        while len(string) > offset:
            for token_class in Lexer.TOKEN_CLASSES:
                result = token_class.match(string,offset)
                if result:
                    new_token = result[0]
                    new_token.source = Source(source_file,line_number)
                    offset += result[1]
                    line_number += result[2]
                
                    do_not_append = False
                    if new_token.type == Token.NEWLINE:
                        if len(tokens) > 2:
                            if tokens[-1].type == Token.IDENTIFIER and tokens[-1].value == "_":
                                tokens = tokens[:-1]
                                do_not_append = True
                                
                    if new_token.type == Token.INCLUDE_FILE:
                        tokens += self.get_included_file(source_file,new_token.value)
                        do_not_append = True     
                        
                    if new_token.type == Token.DIRECTIVE:
                        if new_token.value == "include-once":
                            self.file_skip_filter.add(source_file)  
                            do_not_append = True       
                
                    if new_token.type != Token.WHITESPACE and new_token.type != Token.COMMENT and not do_not_append:
                        tokens.append(new_token)



                    break
            else:
                raise LexError("Could not scan string: "+string[offset:],Source(source_file,line_number))
        eof=EOFToken()
        eof.source = Source(source_file,line_number)
        tokens.append(eof)    
    
        return tokens