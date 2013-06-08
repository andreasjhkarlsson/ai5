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

class AssignGlobalConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_GLOBAL_CONST) + self.identifier.to_binary()

class AssignLocalConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_LOCAL_CONST) + self.identifier.to_binary()
class AssignNearestConstInstruction(Instruction):
    def __init__(self,identifier):
        self.identifier = identifier
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.ASSIGN_NEAREST_CONST) + self.identifier.to_binary()
class PushFunctionInstruction(Instruction):
    def __init__(self,address):
        self.address = address
    def to_binary(self):
        return self.to_binary_with_int_arg(InstructionType.PUSH_FUNCTION, self.address.value)    

class DoubleTopTwoInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.DOUBLE_TOP_TWO)

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

class LoadArgumentInstruction(Instruction):
    def __init__(self,identifier,as_const):
        self.identifier = identifier
        self.as_const = as_const
    def to_binary(self):
        if self.as_const:
            return self.to_binary_without_arg(InstructionType.LOAD_CONST_ARGUMENT) + self.identifier.to_binary()
        else:
            return self.to_binary_without_arg(InstructionType.LOAD_ARGUMENT) + self.identifier.to_binary()

class LoadByRefArgumentInstruction(Instruction):
    def __init__(self,identifier,as_const):
        self.identifier = identifier
        self.as_const = as_const
    def to_binary(self):
        if self.as_const:
            return self.to_binary_without_arg(InstructionType.LOAD_CONST_BYREF_ARGUMENT) + self.identifier.to_binary()
        else:
            return self.to_binary_without_arg(InstructionType.LOAD_BYREF_ARGUMENT) + self.identifier.to_binary()
    
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

class StrongStringEqualInstruction(Instruction):
    def to_binary(self):
        return self.to_binary_without_arg(InstructionType.STRONG_STRING_EQUAL)

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