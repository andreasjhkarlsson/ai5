#include "Instruction.h"
#include <iostream>

std::ostream& operator<< (std::ostream& stream, const Instruction& inst)
{
	switch(inst.type)
	{
	case Instruction::NOOP:
		stream << "NOOP";
		break;
	case Instruction::PUSH_NAME:
		stream << "PUSH_NAME";
		break;
	case Instruction::PUSH_LARGE_INTEGER:
		stream << "PUSH_LARGE_INTEGER";
		break;
	case Instruction::PUSH_SMALL_INTEGER:
		stream << "PUSH_SMALL_INTEGER";
		break;
	case Instruction::PUSH_FLOATING:
		stream << "PUSH_FLOATING";
		break;
	case Instruction::PUSH_BOOLEAN:
		stream << "PUSH_BOOLEAN";
		break;
	case Instruction::PUSH_STRING:
		stream << "PUSH_STRING";
		break;
	case Instruction::PUSH_FUNCTION:
		stream << "PUSH_FUNCTION";
		break;
	case Instruction::PUSH_NULL:
		stream << "PUSH_NULL";
		break;
	case Instruction::INDEX:
		stream << "INDEX";
		break;
	case Instruction::PROPERTY:
		stream << "PROPERTY";
		break;
	case Instruction::POP:
		stream << "POP";
		break;
	case Instruction::CREATE_GLOBAL:
		stream << "CREATE_GLOBAL";
		break;
	case Instruction::CREATE_LOCAL:
		stream << "CREATE_LOCAL";
		break;
	case Instruction::ASSIGN_NAME:
		stream << "ASSIGN_NAME";
		break;
	case Instruction::ASSIGN_INDEX:
		stream << "ASSIGN_INDEX";
		break;
	case Instruction::ASSIGN_PROPERTY:
		stream << "ASSIGN_PROPERTY";
		break;
	case Instruction::JUMP_LONG_ABSOLUTE:
		stream << "JUMP_LONG_ABSOLUTE";
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE:
		stream << "JUMP_SHORT_ABSOLUTE";
		break;
	case Instruction::JUMP_LONG_RELATIVE:
		stream << "JUMP_LONG_RELATIVE";
		break;
	case Instruction::JUMP_SHORT_RELATIVE:
		stream << "JUMP_SHORT_RELATIVE";
		break;
	case Instruction::JUMP_LONG_ABSOLUTE_IF_TRUE:
		stream << "JUMP_LONG_ABSOLUTE_IF_TRUE";
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE_IF_TRUE:
		stream << "JUMP_SHORT_ABSOLUTE_IF_TRUE";
		break;
	case Instruction::JUMP_LONG_RELATIVE_IF_TRUE:
		stream << "JUMP_LONG_RELATIVE_IF_TRUE";
		break;
	case Instruction::JUMP_SHORT_RELATIVE_IF_TRUE:
		stream << "JUMP_SHORT_RELATIVE_IF_TRUE";
		break;
	case Instruction::JUMP_LONG_ABSOLUTE_IF_FALSE:
		stream << "JUMP_LONG_ABSOLUTE_IF_TRUE";
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE_IF_FALSE:
		stream << "JUMP_SHORT_ABSOLUTE_IF_TRUE";
		break;
	case Instruction::JUMP_LONG_RELATIVE_IF_FALSE:
		stream << "JUMP_LONG_RELATIVE_IF_FALSE";
		break;
	case Instruction::JUMP_SHORT_RELATIVE_IF_FALSE:
		stream << "JUMP_SHORT_RELATIVE_IF_FALSE";
		break;
	case Instruction::NEGATION:
		stream << "NEGATION";
		break;
	case Instruction::CALL_FUNCTION:
		stream << "CALL_FUNCTION";
		break;
	case Instruction::SWAP_TOP:
		stream << "SWAP_TOP";
		break;
	case Instruction::DOUBLE_TOP:
		stream << "DOUBLE_TOP";
		break;
	case Instruction::RET:
		stream << "RET";
		break;
	case Instruction::PUSH_EXCEPTION_HANDLER:
		stream << "PUSH_EXCEPTION_HANDLER";
		break;
	case Instruction::POP_EXCEPTION_HANDLER:
		stream << "POP_EXCEPTION_HANDLER";
		break;
	case Instruction::RAISE_EXCEPTION:
		stream << "RAISE_EXCEPTION";
		break;
	case Instruction::BOOLEAN_NOT:
		stream << "BOOLEAN_NOT";
		break;
	case Instruction::BOOLEAN_OR:
		stream << "BOOLEAN_OR";
		break;
	case Instruction::BOOLEAN_AND:
		stream << "BOOLEAN_AND";
		break;
	case Instruction::ADDITION:
		stream << "ADDITION";
		break;
	case Instruction::SUBTRACTION:
		stream << "SUBTRACTION";
		break;
	case Instruction::MULTIPLICATION:
		stream << "MULTIPLICATION";
		break;
	case Instruction::DIVISION:
		stream << "DIVISION";
		break;
	case Instruction::GREATER:
		stream << "GREATER";
		break;
	case Instruction::GREATER_EQUAL:
		stream << "GREATER_EQUAL";
		break;
	case Instruction::LESSER:
		stream << "LESSER";
		break;
	case Instruction::LESSER_EQUAL:
		stream << "LESSER_EQUAL";
		break;
	case Instruction::EQUAL:
		stream << "EQUAL";
		break;
	case Instruction::STRONG_EQUAL:
		stream << "STRONG_EQUAL";
		break;
	case Instruction::NOT_EQUAL:
		stream << "NOT_EQUAL";
		break;
	case Instruction::CONCAT:
		stream << "CONCAT";
		break;
	case Instruction::POW:
		stream << "POW";
		break;
	case Instruction::TERMINATE:
		stream << "TERMINATE";
		break;
		break;

	}
	return stream;
}