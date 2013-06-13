from cx_Freeze import setup, Executable
 
setup(
    name = "AI5Compiler",
    version = "0.1",
    description = "Compiler for the AI5 scripting language.",
    executables = [Executable("ai5.py")]
    )