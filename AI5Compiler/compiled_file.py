
import runtime_types
import struct
import binascii

class Header():
    SIZE = 28
    def __init__(self,magic,compiler_version,target_version,
                 entry_instruction,instructions_start,num_instructions,
                 statics_start,num_statics):
        self.magic = magic
        self.compiler_version = compiler_version
        self.target_version = target_version
        self.entry_instruction = entry_instruction
        self.instructions_start = instructions_start
        self.num_instructions = num_instructions
        self.statics_start = statics_start
        self.num_statics = num_statics
    def to_binary(self):
        return struct.pack("IHHIIIII",self.magic,self.compiler_version,
                           self.target_version,self.entry_instruction,
                           self.instructions_start,self.num_instructions,
                           self.statics_start,self.num_statics)
        


class Instructions:
    def __init__(self,instructions):
        self.instructions = instructions
    def to_binary(self):
        binary = b""
        for instruction in self.instructions:
            binary += instruction.to_binary()
        return binary

class CompiledFile:
    def __init__(self,statics_table,instructions):
        self.statics_table = statics_table
        self.instructions = instructions
    def write_to_file(self,open_file):
        statics_binary = self.statics_table.to_binary()
        instructions_binary = Instructions(self.instructions).to_binary()
        header_binary = Header(0xFEEFDEEF,0,0,0,
                               Header.SIZE+len(statics_binary),len(self.instructions),
                               Header.SIZE,self.statics_table.length()).to_binary()
        open_file.write(header_binary)
        open_file.write(statics_binary)
        open_file.write(instructions_binary)
        
        
        