from lexer import lex_string, LexError
from rd_parser import Parser, Program, ParseError
from compiler import Compiler
from compiled_file import CompiledFile


input = """

const a = 10


"""
try:
    tokens = lex_string(input)
    ast = Parser(tokens).expectRule(Program)
    compiler = Compiler()
    instructions = compiler.compile_program(ast)
    print(instructions)
    statics_table = compiler.static_table

    CompiledFile(statics_table,instructions).write_to_file(open("test.aic","wb"))
except (LexError,ParseError) as error:
    print("Error compiling program:\n\t"+error.message)
    