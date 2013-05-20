#include "Instruction.h"
#include <iostream>


const std::wstring& getName(StackMachine* machine,int index)
{
	StaticData* staticData = machine->getStaticData(index);
	return static_cast<StaticName*>(staticData)->getName();
}

std::wostream& Instruction::format(std::wostream& stream,StackMachine* machine)
{
	switch(this->type)
	{
	case Instruction::NOOP:
		stream << "NOOP";
		break;
	case Instruction::PUSH_NAME:
		stream << "PUSH_NAME" << " " << getName(machine,this->arg.integer);
		break;
	case Instruction::PUSH_LARGE_INTEGER:
		stream << "PUSH_LARGE_INTEGER" << " " << this->arg.int64;
		break;
	case Instruction::PUSH_SMALL_INTEGER:
		stream << "PUSH_SMALL_INTEGER" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::PUSH_FLOATING:
		stream << "PUSH_FLOATING" << " " << this->arg.floating;
		break;
	case Instruction::PUSH_BOOLEAN:
		stream << "PUSH_BOOLEAN" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::PUSH_STRING:
		stream << "PUSH_STRING" << " " << this->arg.integer;
		break;
	case Instruction::PUSH_FUNCTION:
		stream << "PUSH_FUNCTION" << " " << this->arg.integer;
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
	case Instruction::ASSIGN_GLOBAL:
		stream << "ASSIGN_GLOBAL" << " " << getName(machine,this->arg.integer);
		break;
	case Instruction::ASSIGN_LOCAL:
		stream << "ASSIGN_LOCAL" << " " << getName(machine,this->arg.integer);
		break;
	case Instruction::ASSIGN_NEAREST:
		stream << "ASSIGN_NEAREST" << " " << getName(machine,this->arg.integer);
		break;
	case Instruction::ASSIGN_INDEX:
		stream << "ASSIGN_INDEX";
		break;
	case Instruction::ASSIGN_PROPERTY:
		stream << "ASSIGN_PROPERTY" << " " << getName(machine,this->arg.integer);
		break;
	case Instruction::JUMP_LONG_ABSOLUTE:
		stream << "JUMP_LONG_ABSOLUTE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE:
		stream << "JUMP_SHORT_ABSOLUTE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_RELATIVE:
		stream << "JUMP_LONG_RELATIVE"  << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_RELATIVE:
		stream << "JUMP_SHORT_RELATIVE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_ABSOLUTE_IF_TRUE:
		stream << "JUMP_LONG_ABSOLUTE_IF_TRUE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE_IF_TRUE:
		stream << "JUMP_SHORT_ABSOLUTE_IF_TRUE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_RELATIVE_IF_TRUE:
		stream << "JUMP_LONG_RELATIVE_IF_TRUE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_RELATIVE_IF_TRUE:
		stream << "JUMP_SHORT_RELATIVE_IF_TRUE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_ABSOLUTE_IF_FALSE:
		stream << "JUMP_LONG_ABSOLUTE_IF_TRUE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE_IF_FALSE:
		stream << "JUMP_SHORT_ABSOLUTE_IF_TRUE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_RELATIVE_IF_FALSE:
		stream << "JUMP_LONG_RELATIVE_IF_FALSE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_RELATIVE_IF_FALSE:
		stream << "JUMP_SHORT_RELATIVE_IF_FALSE" << " " <<  (int)this->arg.byte;
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

	}
	return stream;
}