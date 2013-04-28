
import unittest
from lexer import *

class TestIntegerToken(unittest.TestCase):
    def test_integer_match(self):
        res = IntegerToken.match("123")
        self.assertEqual(res[0].value,123)
        res = IntegerToken.match("0")
        self.assertEqual(res[0].value,0)
    def test_not_match(self):
        res = IntegerToken.match(" 123")
        self.assertFalse(res)
    def test_hex_match(self):
        res = IntegerToken.match("0xAABBCCDD")
        self.assertEqual(res[0].value,2864434397)
        res = IntegerToken.match("0xFF")
        self.assertEqual(res[0].value,255)

class TestWhitespaceToken(unittest.TestCase):
    def test_match(self):
        res = WhitespaceToken.match("    ")
        self.assertEqual(res[0].value,"    ")
    def test_not_match(self):
        res = WhitespaceToken.match("\n")
        self.assertFalse(res)
        res = WhitespaceToken.match("\r\n")
        self.assertFalse(res)
        
class TestLexer(unittest.TestCase):
    def test_empty(self):
        tokens = lex_string("")[:-1]
        self.assertFalse(tokens)
    def test_only_whitespace(self):
        tokens = lex_string("      ")[:-1]
        self.assertFalse(tokens)
    def test_some_numbers(self):
        tokens = lex_string("  234 567    67 657")[:-1]
        self.assertEqual([x.value for x in tokens],[234,567,67,657])
    def test_multiple_strings(self):
        tokens = lex_string("    \"hej\"  \"bl234'zcxnm\"  'lol' ")[:-1]
        self.assertEqual([x.value for x in tokens], ["hej","bl234'zcxnm","lol"])
    def test_boolean_before_ientifier(self):
        tokens = lex_string("false $false")[:-1]
        self.assertEqual([(x.type,x.value) for x in tokens],[(Token.BOOLEAN,False),(Token.IDENTIFIER,"false")])
    def test_lots_of_tokens(self):
        code = "#include-once func hej($hej,$foo) for $i in bla next\n0xfF/12*4+=123\n123.56+'hejsan'\n123+567 ; FUNC ENDFUNC\nabc[.235] hej.foo"
        tokens = lex_string(code)[:-1]
        self.assertEqual([(x.type,x.value) for x in tokens],[(Token.DIRECTIVE,"include-once"),
                                                             (Token.KEYWORD,KeywordToken.FUNC),
                                                             (Token.IDENTIFIER,"hej"),
                                                             (Token.LEFT_PAREN,"("),
                                                             (Token.IDENTIFIER,"hej"),
                                                             (Token.COMMA,","),
                                                             (Token.IDENTIFIER,"foo"),
                                                             (Token.RIGHT_PAREN,")"),
                                                             (Token.KEYWORD,KeywordToken.FOR),
                                                             (Token.IDENTIFIER,"i"),
                                                             (Token.KEYWORD,KeywordToken.IN),
                                                             (Token.IDENTIFIER,"bla"),
                                                             (Token.KEYWORD,KeywordToken.NEXT),
                                                             (Token.NEWLINE,"\n"),
                                                             (Token.INTEGER,255),
                                                             (Token.OPERATOR,"/"),
                                                             (Token.INTEGER,12),
                                                             (Token.OPERATOR,"*"),
                                                             (Token.INTEGER,4),
                                                             (Token.OPERATOR,"+="),
                                                             (Token.INTEGER,123),
                                                             (Token.NEWLINE,"\n"),
                                                             (Token.FLOATING,123.56),
                                                             (Token.OPERATOR,"+"),
                                                             (Token.STRING,"hejsan"),
                                                             (Token.NEWLINE,"\n"),
                                                             (Token.INTEGER,123),
                                                             (Token.OPERATOR,"+"),
                                                             (Token.INTEGER,567),
                                                             (Token.NEWLINE,"\n"),
                                                             (Token.IDENTIFIER,"abc"),
                                                             (Token.LEFT_BRACKET,"["),
                                                             (Token.FLOATING,.235),
                                                             (Token.RIGHT_BRACKET,"]"),
                                                             (Token.IDENTIFIER,"hej"),
                                                             (Token.DOT,"."),
                                                             (Token.IDENTIFIER,"foo"),])
        
class TestStringToken(unittest.TestCase):
    def test_double_quote_match(self):
        res = StringToken.match("\"hej\"")
        self.assertEqual(res[0].value,"hej")
    def test_single_quote_match(self):
        res = StringToken.match("'hej'")
        self.assertEqual(res[0].value, "hej")
    def test_escape(self):
        res = StringToken.match(r'''"hej\"svejs"''')
        self.assertEqual(res[0].value,'hej"svejs')
        res = StringToken.match(r"""'foo\'bar'""")
        self.assertEqual(res[0].value,"foo'bar")
    def test_not_match(self): 
        self.assertFalse(StringToken.match("'hej"))
        self.assertFalse(StringToken.match('"hej'))
        
class TestOperatorToken(unittest.TestCase):
    OPERATORS = ["+","-","*","/","&","=","==","+=","-=","*=","/=","&=","^","<>","<",">","<=",">=","and","or","not"]
    def test_match(self):
        for op in TestOperatorToken.OPERATORS:
            self.assertTrue(OperatorToken.match(op), "Operator not matched: "+op)
            self.assertEqual(OperatorToken.match(op)[0].value, op,"Operator not matched: "+op) 
    def test_case(self):
        self.assertEqual(OperatorToken.match("aNd")[0].value, "and")
        self.assertEqual(OperatorToken.match("or")[0].value, "or")
        self.assertEqual(OperatorToken.match("NoT")[0].value, "not")
    def test_many(self):
        tokens = lex_string("+= - -= and or ^ *= +- < > <> <= >= < < /=")[:-1]
        self.assertEqual([x.value for x in tokens], ["+=","-","-=","and","or","^","*=","+","-","<",">","<>","<=",">=","<","<","/="])
    def test_prefix(self):
        tokens = lex_string("== = == = =")[:-1]
        self.assertEqual([x.value for x in tokens], ["==","=","==","=","="])
    
            
class TestNewlineToken(unittest.TestCase):
    def test_single_ln_match(self):
        res = NewlineToken.match("\n")
        self.assertEqual(res[0].value,"\n")
    def test_single_crlf_match(self):
        res = NewlineToken.match("\r\n")
        self.assertEqual(res[0].value, "\r\n")
    def test_multiple_mixed_match(self):
        tokens = lex_string("\n\r\n\n\r\n\r\n")[:-1]
        self.assertEqual([x.value for x in tokens],["\n","\r\n","\n","\r\n\r\n"])
        
class TestIdentifierToken(unittest.TestCase):
    def test_single_char(self):
        res = IdentifierToken.match("a")
        self.assertEqual(res[0].value, "a")
    def test_underscore(self):
        res = IdentifierToken.match("a_b")
        self.assertEqual(res[0].value, "a_b")
    def test_pound_and_single_char(self):
        res = IdentifierToken.match("$a")
        self.assertEqual(res[0].value, "a")
    def test_pound_and_digit(self):
        res = IdentifierToken.match("$1")
        self.assertEqual(res[0].value, "1")
    def test_only_pound(self):
        res = IdentifierToken.match("$")
        self.assertFalse(res)
    def test_multiple_char(self):
        res = IdentifierToken.match("abc123")
        self.assertEqual(res[0].value, "abc123")
    def test_pund_multiple_char(self):
        res = IdentifierToken.match("$abc123")
        self.assertEqual(res[0].value, "abc123")
        
class TestMacroToken(unittest.TestCase):
    def test_match(self):
        res = MacroToken.match("@autoit")
        self.assertEqual(res[0].value,"autoit")
    def test_single_at(self):
        res = MacroToken.match("@ 123")
        self.assertFalse(res)    

class TestDirectiveToken(unittest.TestCase):
    def test_simple_match(self):
        res = DirectiveToken.match("#include 123")
        self.assertEqual(res[0].value,"include")
    def test_with_hyphen_match(self):
        res = DirectiveToken.match("#include-once")
        self.assertEqual(res[0].value,"include-once")

class TestCommentToken(unittest.TestCase):
    def test_single_line_match(self):
        res = CommentToken.match(";bla bla 123 _ IF AND +=\nkebab")
        self.assertEqual(res[0].value,";bla bla 123 _ IF AND +=")
    def test_multi_line_match(self):
        res = CommentToken.match("#ComMents-start blabla\r\nfoo bar#comments-END")
        self.assertEqual(res[0].value,"#ComMents-start blabla\r\nfoo bar#comments-END")        
        res = CommentToken.match("#cs blabla\r\nfoo bar#ce")
        self.assertEqual(res[0].value,"#cs blabla\r\nfoo bar#ce")    
    
    
    
class TestFloatingToken(unittest.TestCase):
    def test_full(self):
        res = FloatingToken.match("3.14159")
        self.assertEqual(res[0].value,3.14159)
    def test_min(self):
        res = FloatingToken.match(".14159")
        self.assertEqual(res[0].value,.14159)  

class TestBooleanToken(unittest.TestCase):
    def test_true(self):
        res = BooleanToken.match("tRUE")
        self.assertTrue(res[0].value)
    def test_false(self):
        res = BooleanToken.match("fALSe")
        self.assertFalse(res[0].value)
        

class TestLeftParenthesisToken(unittest.TestCase):
    def test_match(self):
        self.assertEqual(LeftParenToken.match("(")[0].value, "(")

class TestRightParenthesisToken(unittest.TestCase):
    def test_match(self):
        self.assertEqual(RightParenToken.match(")")[0].value, ")")
        
class TestLeftBracketToken(unittest.TestCase):
    def test_match(self):
        self.assertEqual(LeftBracketToken.match("[")[0].value, "[")
        
class TestRightBracketToken(unittest.TestCase):
    def test_match(self):
        self.assertEqual(RightBracketToken.match("]")[0].value, "]")
        
class TestDotToken(unittest.TestCase):
    def test_match(self):
        self.assertEqual(DotToken.match(".")[0].value, ".")
        
class TestKeywordToken(unittest.TestCase):
    def test_prefix_match(self):
        tokens = lex_string("ELSEIF ELSE IF EXITLOOP EXIT")[:-1]
        self.assertEqual([x.value for x in tokens],["elseif","else","if","exitloop","exit"])
    def test_name_conflict(self):
        tokens = lex_string("iniread() in")
        self.assertEqual(tokens[0].type,Token.IDENTIFIER)
        self.assertEqual(tokens[3].type,Token.KEYWORD)
        
    def test_match(self):
        self.assertEqual(KeywordToken.match("exit ")[0].value, "exit")
        self.assertEqual(KeywordToken.match("exit\n")[0].value, "exit")
        self.assertEqual(KeywordToken.match("exit")[0].value, "exit")
        
        
        
class TestIncludeFileToken(unittest.TestCase):
    def test_angel_match(self):
        self.assertEqual(IncludeFileToken.match("#include <hej.all>")[0].value, "hej.all")     
    def test_quote_match(self):
        self.assertEqual(IncludeFileToken.match('#include   "hej.all"')[0].value, "hej.all")  
          
