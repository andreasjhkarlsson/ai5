import struct
from runtime_types import InstructionType

def fits_in_char(integer):
    return integer >= -128 and integer <= 127

class Identifier:
    def __init__(self,global_id,local_id,static_id):
        self.global_id = global_id
        self.local_id = local_id
        self.static_id = static_id
    def to_binary(self):
        return struct.pack("=III",self.global_id,self.local_id,self.static_id)
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
class Address:
    RELATIVE = "relative"
    ABSOLUTE = "absolute"
    UNRESOLVED_ABSOLUTE = "unresolved absolute"
    UNRESOLVED_LOOP_JUMP = "unresolved loop jump"
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)

class RelativeAddress(Address):
    type = Address.RELATIVE
    def __init__(self,offset):
        self.value = offset

class AbsoluteAddress(Address):
    type = Address.ABSOLUTE
    def __init__(self,position):
        self.value = position


# Can be used as a placeholder when absolute address is needed but absolute address
# needs to be calculated later.
class UnresolvedAbsoluteAddress(Address):
    type = Address.UNRESOLVED_ABSOLUTE
    def __init__(self,offset):
        self.offset = offset
    def resolve(self,position):
        return AbsoluteAddress(position + self.offset)


class UnresolvedLoopJumpAddress(Address):
    type = Address.UNRESOLVED_LOOP_JUMP
    TARGET_START = "start"
    TARGET_END  = "end"
    def __init__(self,target,level):
        self.target = target
        self.level = level
    def pop_level(self):
        self.level -= 1
        return self.level
    def resolve(self,position,start_pos,end_pos):
        if self.target == UnresolvedLoopJumpAddress.TARGET_START:
            return RelativeAddress((-position)+start_pos)
        if self.target == UnresolvedLoopJumpAddress.TARGET_END:
            return RelativeAddress((-position)+end_pos)



class Instruction:
    def __repr__(self):
        return "%s(%r)" % (self.__class__.__name__, self.__dict__)
    def to_binary_without_arg(self,type):
        return struct.pack("=B",type)
    def to_binary_with_char_arg(self,type,char):
        return struct.pack("=Bb",type,char)
    def to_binary_with_int_arg(self,type,int):
        return struct.pack("=Bi",type,int)
    def to_binary_with_int64_arg(self,type,int64):
        return struct.pack("=Bq",type,int64)
    def to_binary_with_double_arg(self,type,double):
        return struct.pack("=Bd",type,double)
    def to_binary_with_two_int_arg(self,type,i1,i2):
        return struct.pack("=Bii",type,i1,i2)
    def to_binary_with_two_char_args(self,type,c1,c2):
        return struct.pack("=Bbb",type,c1,c2)

class IndexInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.INDEX)

class CreateMultiDimListInstruction(Instruction):
    def __init__(self,subscripts):
        self.subscripts = subscripts
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.CREATE_MULTIDIM_LIST,self.subscripts)
class RedimMultiDimListInstruction(Instruction):
    def __init__(self,subscripts):
        self.subscripts = subscripts
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.REDIM_MULTIDIM_LIST,self.subscripts)

class PushInteger32Instruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_INTEGER32, self.id)

class PushInteger64Instruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_INTEGER64, self.id)

class MakeGlobalConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.MAKE_GLOBAL_CONST) + self.identifier.to_binary()

class MakeLocalConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.MAKE_LOCAL_CONST) + self.identifier.to_binary()
class MakeNearestConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.MAKE_NEAREST_CONST) + self.identifier.to_binary()
class PushFunctionInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_FUNCTION, self.address.value)    

class DoubleTopTwoInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.DOUBLE_TOP_TWO)

class DoubleTopInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.DOUBLE_TOP)

class SwapTopInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.SWAP_TOP)

class AssignIndexInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_INDEX)

class PushStringInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_STRING, self.id)
    
class PushNameValueInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.PUSH_NAME_VALUE) + self.identifier.to_binary()


class PushFloatingInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_FLOATING,self.id)

class PushMacroInstruction(Instruction):
    def __init__(self,id):
        self.id = id
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_MACRO,self.id)

class PopInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.POP)
    
class PushNullInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.PUSH_NULL)

class PushDefaultInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.PUSH_DEFAULT)

class BuildListInstruction(Instruction):
    def __init__(self,count):
        self.count = count
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.BUILD_LIST,self.count)

class PushBooleanInstruction(Instruction):
    def __init__(self,value):
        self.value=value
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.PUSH_BOOLEAN,self.value)
    
class RetInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.RET)
    
class AssignNearestInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_NEAREST) + self.identifier.to_binary()
    
class AssignGlobalInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_GLOBAL) + self.identifier.to_binary()
    
class AssignLocalInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_LOCAL) + self.identifier.to_binary()

class CreateArgumentInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.CREATE_ARGUMENT) + self.identifier.to_binary()

class CreateByRefArgumentInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.CREATE_BYREF_ARGUMENT) + self.identifier.to_binary()
    
class LoadArgumentsInstruction(Instruction):
    def __init__(self,total,required):
        self.total = total
        self.required = required
    def to_binary(self):
        return self.to_binary_with_two_char_args(InstructionType.LOAD_ARGUMENTS,self.total,self.required)

class AdditionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ADDITION)

class ConcatInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.CONCAT)

class GreaterInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.GREATER)
class GreaterEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.GREATER_EQUAL)
class LesserInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.LESSER)
class LesserEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.LESSER_EQUAL)
class EqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.EQUAL)

class NotEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.NOT_EQUAL)

class StrongStringEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.STRONG_STRING_EQUAL)

class ExactlyEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.EXACTLY_EQUAL)

class MultiplicationInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.MULTIPLICATION)

class DivisionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.DIVISION)

class SubtractionInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.SUBTRACTION)

class PowInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.POW)

class NegationInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.NEGATION)

class BooleanNotInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.BOOLEAN_NOT)

class BooleanAndInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.BOOLEAN_AND)

class BooleanOrInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.BOOLEAN_OR)
    
class JumpInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        if fits_in_char(self.address.value):
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_RELATIVE, self.address.value)
            else:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_ABSOLUTE, self.address.value)
        else:
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_RELATIVE, self.address.value)
            else:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_ABSOLUTE, self.address.value)
        
class JumpIfFalseInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        if fits_in_char(self.address.value):
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_RELATIVE_IF_FALSE, self.address.value)
            else:
                return self.to_binary_with_char_arg(InstructionType.JUMP_SHORT_ABSOLUTE_IF_FALSE, self.address.value)
        else:
            if self.address.type == Address.RELATIVE:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_RELATIVE_IF_FALSE, self.address.value)
            else:
                return self.to_binary_with_int_arg(InstructionType.JUMP_LONG_ABSOLUTE_IF_FALSE, self.address.value)
        
class TerminateInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.TERMINATE)
    
class CallFunctionInstruction(Instruction):
    def __init__(self,num_arguments):
        self.num_arguments = num_arguments
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.CALL_FUNCTION,self.num_arguments)   

class PushLoopBlockInstruction(Instruction):
    def __init__(self,continue_address,exit_address):
        self.continue_address = continue_address
        self.exit_address = exit_address
    def to_binary(self):
        return self.to_binary_with_two_int_arg(InstructionType.PUSH_LOOP_BLOCK,self.continue_address.value,self.exit_address.value) 

class PopBlockInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.POP_BLOCK)

class PushGeneralBlockInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.PUSH_GENERAL_BLOCK)

class BreakLoopInstruction(Instruction):
    def __init__(self,level):
        self.level=level
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.BREAK_LOOP,self.level)

class ContinueLoopInstruction(Instruction):
    def __init__(self,level):
        self.level=level
    def to_binary(self):
        return self.to_binary_with_char_arg(InstructionType.CONTINUE_LOOP,self.level)