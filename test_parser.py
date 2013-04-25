
import unittest
from lexer import lex_string
from rd_parser import *



def match_rule(code,rule):
    p = Parser(lex_string(code))
    return p.expectRule(rule)

class TestParser(unittest.TestCase):
    def test_accept_token(self):
        self.assertTrue(Parser(lex_string("123")).accept(Token.INTEGER))
        self.assertTrue(Parser(lex_string("123")).accept(Token.INTEGER,123))
        self.assertFalse(Parser(lex_string("123")).accept(Token.INTEGER,321))
        self.assertFalse(Parser(lex_string("123.45")).accept(Token.INTEGER))
    def test_current_token(self):
        p = Parser(lex_string("123 456"))
        p.accept(Token.INTEGER)
        self.assertEqual(p.current.value,123)
        p.accept(Token.STRING)
        self.assertEqual(p.current.value,123)
    def test_expect_token(self):
        self.assertTrue(Parser(lex_string("123")).expect(Token.INTEGER))
        self.assertTrue(Parser(lex_string("123")).expect(Token.INTEGER,123))
        self.assertRaises(Exception, Parser(lex_string("123")).expect,Token.IDENTIFIER,321)
        self.assertRaises(Exception, Parser(lex_string("123")).expect,Token.IDENTIFIER)
    def test_accept_rule(self):
        self.assertTrue(Parser(lex_string("#include-once")).acceptRule(Directive))
        self.assertFalse(Parser(lex_string("#include-once")).acceptRule(Function))
    def test_expect_rule(self):
        self.assertTrue(Parser(lex_string("#include-once")).expectRule(Directive))
        self.assertRaises(Exception, Parser(lex_string("#include-once")).expectRule,Function)
    def test_accept_multiple_tokens(self):
        p = Parser(lex_string("1 2 3 4"))
        p.accept_multiple_tokens([[Token.INTEGER],[Token.FLOATING]])
        self.assertEqual(p.current,None)
        
        p.accept(Token.INTEGER)
        p.accept_multiple_tokens([[Token.INTEGER],[Token.INTEGER],[Token.INTEGER,666]])
        self.assertEqual(p.current.value,1)
        
        matched = p.accept_multiple_tokens([[Token.INTEGER],[Token.INTEGER,3],[Token.INTEGER]])
        self.assertEqual([x.value for x in matched],[2,3,4])
        
        
        




class TestIncludeRule(unittest.TestCase):
    def test_bracket_match(self):
        i = match_rule("#include <file.ext>",Include)
        self.assertEqual(i.token.value,"file.ext")
    def test_quote_match(self):
        i = match_rule('#include "file.ext"',Include)
        self.assertEqual(i.token.value,"file.ext")
    def test_non_match(self):
        self.assertRaises(Exception,match_rule,"#include",Include)
        
class TestFunctionRule(unittest.TestCase):
    def test_empty_function(self):
        self.assertTrue(match_rule("func a()\nendfunc",Function))
    def test_non_empty_function(self):
        self.assertTrue(match_rule("func a()\n12+5-5\nendfunc",Function))
    def test_function_name(self):
        self.assertEqual(match_rule("func abc()\nendfunc",Function).name.value,"ABC")
    def test_single_argument(self):
        self.assertEqual(match_rule("func a(b)\nendfunc",Function).arguments[0].value,"B")
    def test_multiple_arguments(self):
        self.assertEqual([x.value for x in match_rule("func a(b,c,d)\nendfunc",Function).arguments],["B","C","D"])
        
        