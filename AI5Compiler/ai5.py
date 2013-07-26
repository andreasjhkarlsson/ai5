from lexer import Lexer, LexError
from rd_parser import Parser, Program, ParseError
from compiler import Compiler, CompileError
from compiled_file import CompiledFile
import binascii
import sys
import os
import codecs

args = sys.argv[1:]

if len(args) == 0:
    print("Usage: ai5 <input_file> [<output_file>]")
    sys.exit(0)

if len(args) < 1 or len(args) > 2:
    print("Wrong number of arguments to compiler")
    sys.exit(1)

input_file = args[0]
if len(args) == 2:
    output_file = args[1]
else:
    fileName, fileExtension = os.path.splitext(input_file)
    output_file = fileName + ".aic"



try:


    fhandle = codecs.open(input_file,"r","utf-8")
    input = fhandle.read()
    fhandle.close()

    lexer = Lexer(os.path.dirname(sys.argv[0] )+"/ai5-include/")
    tokens = lexer.lex_string(input,input_file)

    parser = Parser(tokens)
    ast = parser.parse_program()



    compiler = Compiler()

    instructions = compiler.compile_program(ast)

    statics_table = compiler.static_table
    #statics_table.dump()
    #for index,instruction in enumerate(instructions):
    #   print(index,":",instruction,binascii.hexlify(instruction.to_binary()))
    

    CompiledFile(statics_table,instructions).write_to_file(open(output_file,"wb"))

    #print("Compiled program written to file "+output_file+" without problems.")
except (LexError,ParseError,CompileError) as error:
    print("Error compiling program:\n\tin file: "+error.source.filename+" at line "+str(error.source.line_number)+": "+error.message)
    sys.exit(1)
    