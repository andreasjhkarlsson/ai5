
import unittest
from lexer import lex_string
from rd_parser import *



def match_rule(code,rule):
    p = Parser(lex_string(code))
    return p.expectRule(p)

class TestParser(unittest.TestCase):
    def test_accept_token(self):
        self.assertTrue(Parser(lex_string("123")).accept(Token.INTEGER))
        self.assertTrue(Parser(lex_string("123")).accept(Token.INTEGER,123))
        self.assertFalse(Parser(lex_string("123.45")).accept(Token.INTEGER))
    def test_current_token(self):
        p = Parser(lex_string("123 456"))
        p.accept(Token.INTEGER)
        self.assertEqual(p.current.value,123)
        p.accept(Token.STRING)
        self.assertEqual(p.current.value,123)
    def test_accept_rule(self):
        self.assertTrue(Parser(lex_string("#include-once")).acceptRule(Directive))
        self.assertFalse(Parser(lex_string("#include-once")).acceptRule(Function))
    def test_expect_rule(self):