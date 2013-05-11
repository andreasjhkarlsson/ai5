from lexer import lex_string
from rd_parser import Parser, Program
from compiler import Compiler
from compiled_file import CompiledFile


input = """
foo = 123+456+789
"""

tokens = lex_string(input)
ast = Parser(tokens).expectRule(Program)
compiler = Compiler()
instructions = compiler.compile_program(ast)
statics_table = compiler.static_table

CompiledFile(statics_table,instructions).write_to_file(open("test.aic","wb"))

