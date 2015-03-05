# AI5

Compiler and runtime for an extended version of the AutoIt3 language

The project is divided into two parts:

* A custom written lexer, parser and compiler in python 3 that compiles AutoIt3 source code to bytecode. The syntax of the language has been extended to provide classes, hashmaps, closures and more.
* A runtime written in C++ that runs the code produced by the python compiler. The runtime is multi-threaded, garbage collected and is optimized for speed.
