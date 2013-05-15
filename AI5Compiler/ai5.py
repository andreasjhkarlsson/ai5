from lexer import lex_string
from rd_parser import Parser, Program
from compiler import Compiler
from compiled_file import CompiledFile


input = """
func subtract()
i = i -1
endfunc
i = 100000000
while i > 0
subtract()
wend
printline(i)
"""

tokens = lex_string(input)
ast = Parser(tokens).expectRule(Program)
compiler = Compiler()
instructions = compiler.compile_program(ast)
print(instructions)
statics_table = compiler.static_table

CompiledFile(statics_table,instructions).write_to_file(open("test.aic","wb"))

