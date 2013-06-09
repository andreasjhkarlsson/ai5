#include "Instruction.h"
#include <iostream>
#include "types.h"


shared_string getName(StackMachine* machine,NameIdentifier nameId)
{
	StaticData* staticData = machine->getStaticData(nameId.staticId);
	return static_cast<StaticName*>(staticData)->getName();
}

shared_string getString(StackMachine* machine,int index)
{
	StaticData* staticData = machine->getStaticData(index);
	return static_cast<StaticString*>(staticData)->getVariant()->toString();
}

shared_string getMacro(StackMachine* machine,int index)
{
	StaticData* staticData = machine->getStaticData(index);
	return static_cast<StaticMacro*>(staticData)->getName();
}

double getFloating(StackMachine* machine,int index)
{
	StaticData* staticData = machine->getStaticData(index);
	return static_cast<StaticFloating*>(staticData)->getVariant()->getValue();
}

__int64 getInteger64(StackMachine* machine,int index)
{
	StaticData* staticData = machine->getStaticData(index);
	return static_cast<StaticInteger64*>(staticData)->getVariant()->getValue();
}

int getInteger32(StackMachine* machine,int index)
{
	StaticData* staticData = machine->getStaticData(index);
	return static_cast<StaticInteger32*>(staticData)->getVariant()->toInteger32();
}


std::wostream& Instruction::format(std::wostream& stream,StackMachine* machine)
{
	switch(this->type)
	{
	case Instruction::NOOP:
		stream << "NOOP";
		break;
	case Instruction::PUSH_NAME_VALUE:
		stream << "PUSH_NAME_VALUE" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::PUSH_INTEGER64:
		stream << "PUSH_INTEGER64" << " " << getInteger64(machine, this->arg.integer);
		break;
	case Instruction::PUSH_INTEGER32:
		stream << "PUSH_INTEGER32" << " " << getInteger32(machine, this->arg.integer);
		break;
	case Instruction::PUSH_FLOATING:
		stream << "PUSH_FLOATING" << " " << getFloating(machine, this->arg.integer);
		break;
	case Instruction::PUSH_BOOLEAN:
		stream << "PUSH_BOOLEAN" << " ";
		if(this->arg.byte)
			stream << "true";
		else
			stream << "false";
		break;
	case Instruction::PUSH_STRING:
		stream << "PUSH_STRING" << " " << *getString(machine,this->arg.integer);
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
	case Instruction::MAKE_GLOBAL_CONST:
		stream << "MAKE_GLOBAL_CONST" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::MAKE_LOCAL_CONST:
		stream << "MAKE_LOCAL_CONST" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::MAKE_NEAREST_CONST:
		stream << "MAKE_NEAREST_CONST" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::ASSIGN_GLOBAL:
		stream << "ASSIGN_GLOBAL" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::ASSIGN_LOCAL:
		stream << "ASSIGN_LOCAL" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::ASSIGN_NEAREST:
		stream << "ASSIGN_NEAREST" << " " << *getName(machine,this->arg.identifier);
		break;
	case Instruction::ASSIGN_INDEX:
		stream << "ASSIGN_INDEX";
		break;
	case Instruction::ASSIGN_PROPERTY:
		stream << "ASSIGN_PROPERTY" << " " << *getName(machine,this->arg.identifier);
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
		stream << "CALL_FUNCTION" << " " << (int)this->arg.byte;
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
	case Instruction::STRONG_STRING_EQUAL:
		stream << "STRONG_STRING_EQUAL";
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
	case Instruction::BUILD_LIST:
		stream << "BUILD_LIST "<<arg.integer;
		break;
	case Instruction::CREATE_MULTIDIM_LIST:
		stream << "CREATE_MULTIDIM_LIST " << (int)arg.byte;
		break;
	case Instruction::REDIM_MULTIDIM_LIST:
		stream << "REDIM_MULTIDIM_LIST " << (int)arg.byte;
		break;
	case Instruction::PUSH_MACRO:
		stream << "PUSH_MACRO " << *getMacro(machine,arg.integer);
		break;
	case Instruction::DOUBLE_TOP_TWO:
		stream << "DOUBLE_TOP_TWO";
		break;
	case Instruction::PUSH_LOOP_BLOCK:
		stream << "PUSH_LOOP_BLOCK " << arg.integerPair.i1 << " " << arg.integerPair.i2;
		break;
	case Instruction::POP_BLOCK:
		stream << "POP_LOOP_BLOCK";
		break;
	case Instruction::CONTINUE_LOOP:
		stream << "CONTINUE_LOOP " << (int)arg.byte;
		break;
	case Instruction::BREAK_LOOP:
		stream << "BREAK_LOOP " << (int)arg.byte;
		break;
	case Instruction::CREATE_ARGUMENT:
		stream << "CREATE_ARGUMENT " << *getName(machine,arg.identifier);
		break;
	case Instruction::CREATE_BYREF_ARGUMENT:
		stream << "CREATE_BYREF_ARGUMENT " << *getName(machine,arg.identifier);
		break;
	case Instruction::LOAD_ARGUMENTS:
		stream << "LOAD_ARGUMENTS " << (int)arg.bytePair.b1 << " " << (int)arg.bytePair.b2;
		break;
	default:
		stream << "No pretty print available for instruction.";
		break;
	}
	return stream;
}