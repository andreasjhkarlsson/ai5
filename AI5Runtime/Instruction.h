#pragma once

#include <vector>
#include <memory>
#include "StackMachine.h"
#include "operators.h"
#include "stack_instructions.h"
#include "misc_instructions.h"
#include "jump_instructions.h"
#include "comparison_instructions.h"

typedef unsigned char INSTRUCTION_TYPE;



class ProgramLoader;

class Instruction
{
public:
	typedef std::shared_ptr<Instruction> PTR;
	static const INSTRUCTION_TYPE NOOP							= 0x00;
	static const INSTRUCTION_TYPE PUSH_NAME_VALUE				= 0x01;
	static const INSTRUCTION_TYPE PUSH_INTEGER64				= 0x02;
	static const INSTRUCTION_TYPE PUSH_INTEGER32				= 0x03;
	static const INSTRUCTION_TYPE PUSH_FLOATING					= 0x04;
	static const INSTRUCTION_TYPE PUSH_BOOLEAN					= 0x05;
	static const INSTRUCTION_TYPE PUSH_STRING					= 0x06;
	static const INSTRUCTION_TYPE PUSH_FUNCTION					= 0x07;
	static const INSTRUCTION_TYPE PUSH_NULL						= 0x08;
	static const INSTRUCTION_TYPE INDEX							= 0x09;
	static const INSTRUCTION_TYPE PROPERTY						= 0x0A;
	static const INSTRUCTION_TYPE POP							= 0x0B;
	static const INSTRUCTION_TYPE ASSIGN_GLOBAL					= 0x0C;
	static const INSTRUCTION_TYPE ASSIGN_LOCAL					= 0x0D;
	static const INSTRUCTION_TYPE ASSIGN_NEAREST				= 0x0E;
	static const INSTRUCTION_TYPE ASSIGN_INDEX					= 0x0F;
	static const INSTRUCTION_TYPE ASSIGN_PROPERTY				= 0x10;
	static const INSTRUCTION_TYPE JUMP_LONG_ABSOLUTE			= 0x11;
	static const INSTRUCTION_TYPE JUMP_SHORT_ABSOLUTE			= 0x12;
	static const INSTRUCTION_TYPE JUMP_LONG_RELATIVE			= 0x13;
	static const INSTRUCTION_TYPE JUMP_SHORT_RELATIVE			= 0x14;
	static const INSTRUCTION_TYPE JUMP_LONG_ABSOLUTE_IF_TRUE	= 0x15;
	static const INSTRUCTION_TYPE JUMP_SHORT_ABSOLUTE_IF_TRUE	= 0x16;
	static const INSTRUCTION_TYPE JUMP_LONG_RELATIVE_IF_TRUE	= 0x17;
	static const INSTRUCTION_TYPE JUMP_SHORT_RELATIVE_IF_TRUE	= 0x18;
	static const INSTRUCTION_TYPE JUMP_LONG_ABSOLUTE_IF_FALSE	= 0x19;
	static const INSTRUCTION_TYPE JUMP_SHORT_ABSOLUTE_IF_FALSE	= 0x1A;
	static const INSTRUCTION_TYPE JUMP_LONG_RELATIVE_IF_FALSE	= 0x1B;
	static const INSTRUCTION_TYPE JUMP_SHORT_RELATIVE_IF_FALSE	= 0x1C;
	static const INSTRUCTION_TYPE NEGATION						= 0x1D;
	static const INSTRUCTION_TYPE CALL_FUNCTION					= 0x1E;
	static const INSTRUCTION_TYPE SWAP_TOP						= 0x1F;
	static const INSTRUCTION_TYPE DOUBLE_TOP					= 0x20;
	static const INSTRUCTION_TYPE RET							= 0x21;
	static const INSTRUCTION_TYPE PUSH_EXCEPTION_HANDLER		= 0x22;
	static const INSTRUCTION_TYPE POP_EXCEPTION_HANDLER			= 0x23;
	static const INSTRUCTION_TYPE RAISE_EXCEPTION				= 0x24;
	static const INSTRUCTION_TYPE BOOLEAN_NOT					= 0x25;
	static const INSTRUCTION_TYPE BOOLEAN_OR					= 0x26;
	static const INSTRUCTION_TYPE BOOLEAN_AND					= 0x27;
	static const INSTRUCTION_TYPE ADDITION						= 0x28;
	static const INSTRUCTION_TYPE SUBTRACTION					= 0x29;
	static const INSTRUCTION_TYPE MULTIPLICATION				= 0x2A;
	static const INSTRUCTION_TYPE DIVISION						= 0x2B;
	static const INSTRUCTION_TYPE GREATER						= 0x2C;
	static const INSTRUCTION_TYPE GREATER_EQUAL					= 0x2D;
	static const INSTRUCTION_TYPE LESSER						= 0x2E;
	static const INSTRUCTION_TYPE LESSER_EQUAL					= 0x2F;
	static const INSTRUCTION_TYPE EQUAL							= 0x30;
	static const INSTRUCTION_TYPE STRONG_STRING_EQUAL			= 0x31;
	static const INSTRUCTION_TYPE NOT_EQUAL						= 0x32;
	static const INSTRUCTION_TYPE CONCAT						= 0x33;
	static const INSTRUCTION_TYPE POW							= 0x34;
	static const INSTRUCTION_TYPE TERMINATE						= 0x35;
	static const INSTRUCTION_TYPE ASSIGN_GLOBAL_CONST			= 0x36;
	static const INSTRUCTION_TYPE ASSIGN_LOCAL_CONST			= 0x37;
	static const INSTRUCTION_TYPE ASSIGN_NEAREST_CONST			= 0x38;
	static const INSTRUCTION_TYPE BUILD_LIST					= 0x3A;
	static const INSTRUCTION_TYPE CREATE_MULTIDIM_LIST			= 0x3B;
	static const INSTRUCTION_TYPE LOAD_ARGUMENT					= 0x3C;
	static const INSTRUCTION_TYPE LOAD_BYREF_ARGUMENT			= 0x3D;
	static const INSTRUCTION_TYPE PUSH_MACRO					= 0x3E;
	static const INSTRUCTION_TYPE DOUBLE_TOP_TWO				= 0x3F;
	
	Instruction(unsigned char type): type(type){}
	__forceinline void execute(StackMachine* machine);
	std::wostream& format(std::wostream& stream,StackMachine* machine);
	friend class ProgramLoader;
private:
	INSTRUCTION_TYPE type;
	union
	{
		char byte;
		int integer;
		double floating;
		__int64 int64;
		NameIdentifier identifier;

	} arg;
};



void Instruction::execute(StackMachine* machine)
{
	// Assume compiler generates jump table for 
	// the instructions.
	// The function calls should all be inlined.
	switch(type)
	{
	case NOOP:
		noop(machine);
		break;
	case JUMP_LONG_RELATIVE:
		jumpLongRelative(machine,arg.integer);
		break;
	case JUMP_LONG_RELATIVE_IF_FALSE:
		jumpLongRelativeIfFalse(machine,arg.integer);
		break;
	case JUMP_LONG_RELATIVE_IF_TRUE:
		jumpLongRelativeIfTrue(machine,arg.integer);
		break;
	case JUMP_SHORT_RELATIVE_IF_FALSE:
		jumpShortRelativeIfFalse(machine,arg.byte);
		break;
	case JUMP_SHORT_RELATIVE:
		jumpShortRelative(machine,arg.byte);
		break;
	case PUSH_INTEGER64:
		pushInteger64(machine,arg.integer);
		break;
	case PUSH_INTEGER32:
		pushInteger32(machine,arg.integer);
		break;
	case POP:
		pop(machine);
		break;
	case TERMINATE:
		terminate(machine);
		break;
	case ADDITION:
		mathOperationInstruction(machine,MATH_OPERATION::ADDITION);
		break;
	case MULTIPLICATION:
		mathOperationInstruction(machine,MATH_OPERATION::MULTIPLICATION);
		break;
	case DIVISION:
		mathOperationInstruction(machine,MATH_OPERATION::DIVISION);
		break;
	case SUBTRACTION:
		mathOperationInstruction(machine,MATH_OPERATION::SUBTRACTION);
		break;
	case POW:
		mathOperationInstruction(machine,MATH_OPERATION::POW);
		break;
	case GREATER:
		comparisonInstruction(machine,COMPARISON_TYPE::GREATER);
		break;
	case GREATER_EQUAL:
		comparisonInstruction(machine,COMPARISON_TYPE::GREATER_EQUAL);
		break;
	case LESSER:
		comparisonInstruction(machine,COMPARISON_TYPE::LESSER);
		break;
	case LESSER_EQUAL:
		comparisonInstruction(machine,COMPARISON_TYPE::LESSER_EQUAL);
		break;
	case EQUAL:
		comparisonInstruction(machine,COMPARISON_TYPE::EQUAL);
		break;
	case STRONG_STRING_EQUAL:
		comparisonInstruction(machine,COMPARISON_TYPE::STRONG_STRING_EQUAL);
		break;
	case CONCAT:
		concatStrings(machine);
		break;
	case DOUBLE_TOP:
		doubleTop(machine);
		break;
	case ASSIGN_GLOBAL:
		assignGlobal(machine,arg.identifier);
		break;
	case ASSIGN_GLOBAL_CONST:
		assignGlobalConst(machine,arg.identifier);
		break;
	case ASSIGN_LOCAL:
		assignLocal(machine,arg.identifier);
		break;
	case ASSIGN_LOCAL_CONST:
		assignLocalConst(machine,arg.identifier);
		break;
	case PUSH_NAME_VALUE:
		pushNameValue(machine,arg.identifier);
		break;
	case PUSH_FUNCTION:
		pushFunction(machine,arg.integer);
		break;
	case ASSIGN_NEAREST:
		assignNearest(machine,arg.identifier);
		break;
	case ASSIGN_NEAREST_CONST:
		assignNearestConst(machine,arg.identifier);
		break;
	case CALL_FUNCTION:
		callFunction(machine,arg.byte);
		break;
	case PUSH_FLOATING:
		pushFloating(machine,arg.integer);
		break;
	case RET:
		ret(machine);
		break;
	case PUSH_NULL:
		pushNull(machine);
		break;
	case PUSH_STRING:
		pushString(machine,arg.integer);
		break;
	case BOOLEAN_NOT:
		booleanNot(machine);
		break;
	case BOOLEAN_AND:
		booleanAnd(machine);
		break;
	case BOOLEAN_OR:
		booleanOr(machine);
		break;
	case PUSH_BOOLEAN:
		pushBoolean(machine,arg.byte);
		break;
	case BUILD_LIST:
		buildList(machine,arg.integer);
		break;
	case INDEX:
		derefIndex(machine);
		break;
	case LOAD_ARGUMENT:
		loadArgument(machine,arg.identifier);
		break;
	case LOAD_BYREF_ARGUMENT:
		loadByRefArgument(machine,arg.identifier);
		break;
	case PUSH_MACRO:
		pushMacro(machine,arg.integer);
		break;
	case ASSIGN_INDEX:
		assignIndex(machine);
		break;
	case DOUBLE_TOP_TWO:
		doubleTopTwo(machine);
		break;
	default:
		throw RuntimeError(L"Unknown instruction detected!");
		break;
	}
}