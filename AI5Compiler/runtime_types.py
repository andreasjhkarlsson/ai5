
class InstructionType:
    NOOP                                = 0x00;
    PUSH_NAME                           = 0x01;
    PUSH_INTEGER                        = 0x02;
    PUSH_FLOATING                       = 0x04;
    PUSH_BOOLEAN                        = 0x05;
    PUSH_STRING                         = 0x06;
    PUSH_FUNCTION                       = 0x07;
    PUSH_NULL                           = 0x08;
    INDEX                               = 0x09;
    PROPERTY                            = 0x0A;
    POP                                 = 0x0B;
    ASSIGN_GLOBAL                       = 0x0C;
    ASSIGN_LOCAL                        = 0x0D;
    ASSIGN_NEAREST                      = 0x0E;
    ASSIGN_INDEX                        = 0x0F;
    ASSIGN_PROPERTY                     = 0x10;
    JUMP_LONG_ABSOLUTE                  = 0x11;
    JUMP_SHORT_ABSOLUTE                 = 0x12;
    JUMP_LONG_RELATIVE                  = 0x13;
    JUMP_SHORT_RELATIVE                 = 0x14;
    JUMP_LONG_ABSOLUTE_IF_TRUE          = 0x15;
    JUMP_SHORT_ABSOLUTE_IF_TRUE         = 0x16;
    JUMP_LONG_RELATIVE_IF_TRUE          = 0x17;
    JUMP_SHORT_RELATIVE_IF_TRUE         = 0x18;
    JUMP_LONG_ABSOLUTE_IF_FALSE         = 0x19;
    JUMP_SHORT_ABSOLUTE_IF_FALSE        = 0x1A;
    JUMP_LONG_RELATIVE_IF_FALSE         = 0x1B;
    JUMP_SHORT_RELATIVE_IF_FALSE        = 0x1C;
    NEGATION                            = 0x1D;
    CALL_FUNCTION                       = 0x1E;
    SWAP_TOP                            = 0x1F;
    DOUBLE_TOP                          = 0x20;
    RET                                 = 0x21;
    PUSH_EXCEPTION_HANDLER              = 0x22;
    POP_EXCEPTION_HANDLER               = 0x23;
    RAISE_EXCEPTION                     = 0x24;
    BOOLEAN_NOT                         = 0x25;
    BOOLEAN_OR                          = 0x26;
    BOOLEAN_AND                         = 0x27;
    ADDITION                            = 0x28;
    SUBTRACTION                         = 0x29;
    MULTIPLICATION                      = 0x2A;
    DIVISION                            = 0x2B;
    GREATER                             = 0x2C;
    GREATER_EQUAL                       = 0x2D;
    LESSER                              = 0x2E;
    LESSER_EQUAL                        = 0x2F;
    EQUAL                               = 0x30;
    STRONG_EQUAL                        = 0x31;
    NOT_EQUAL                           = 0x32;
    CONCAT                              = 0x33;
    POW                                 = 0x34;
    TERMINATE                           = 0x35;
    ASSIGN_GLOBAL_CONST                 = 0x36;
    ASSIGN_LOCAL_CONST                  = 0x37;
    ASSIGN_NEAREST_CONST                = 0x38;
    BUILD_LIST      				    = 0x3A;
    CREATE_MULTIDIM_LIST		        = 0x3B;


class StaticType:
    STRING      = 0x0
    BINARY      = 0x1
    NAME        = 0x2
    FLOATING    = 0x3
    INTEGER     = 0x4
    
    