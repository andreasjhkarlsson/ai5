#include "Instruction.h"
#include <iostream>
#include "types.h"
#include "misc.h"


shared_string getName(shared_ptr<vector<shared_ptr<StaticData>>> statics,NameIdentifier nameId)
{
	StaticData* staticData = ((*statics)[nameId.staticId]).get();
	return static_cast<StaticName*>(staticData)->getName();
}

shared_string getString(shared_ptr<vector<shared_ptr<StaticData>>> statics,int index)
{
	StaticData* staticData = ((*statics)[index]).get();
	return static_cast<StaticString*>(staticData)->getVariant()->toString();
}

shared_string getMacro(shared_ptr<vector<shared_ptr<StaticData>>> statics,int index)
{
	StaticData* staticData = ((*statics)[index]).get();
	return static_cast<StaticMacro*>(staticData)->getName();
}

double getFloating(shared_ptr<vector<shared_ptr<StaticData>>> statics,int index)
{
	StaticData* staticData = ((*statics)[index]).get();
	return static_cast<StaticFloating*>(staticData)->getFloating();
}

__int64 getInteger64(shared_ptr<vector<shared_ptr<StaticData>>> statics,int index)
{
	StaticData* staticData = ((*statics)[index]).get();
	return static_cast<StaticInteger64*>(staticData)->getInt();
}

int getInteger32(shared_ptr<vector<shared_ptr<StaticData>>> statics,int index)
{
	StaticData* staticData =((*statics)[index]).get();
	return static_cast<StaticInteger32*>(staticData)->getInt();
}


void Instruction::print(shared_ptr<vector<shared_ptr<StaticData>>> statics)
{
	switch(this->type)
	{
	case Instruction::NOOP:
		DebugOut(L"Instruction") << "NOOP";
		break;
	case Instruction::PUSH_NAME_VALUE:
		DebugOut(L"Instruction") << "PUSH_NAME_VALUE" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::PUSH_NAME:
		DebugOut(L"Instruction") << "PUSH_NAME" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::PUSH_INTEGER64:
		DebugOut(L"Instruction") << "PUSH_INTEGER64" << " " << getInteger64(statics, this->arg.integer);
		break;
	case Instruction::PUSH_INTEGER32:
		DebugOut(L"Instruction") << "PUSH_INTEGER32" << " " << getInteger32(statics, this->arg.integer);
		break;
	case Instruction::PUSH_FLOATING:
		DebugOut(L"Instruction") << "PUSH_FLOATING" << " " << getFloating(statics, this->arg.integer);
		break;
	case Instruction::PUSH_BOOLEAN:
		DebugOut(L"Instruction") << "PUSH_BOOLEAN" << " " << ((this->arg.byte!=0) ? "True":"False");
		break;
	case Instruction::PUSH_STRING:
		DebugOut(L"Instruction") << "PUSH_STRING" << " " << getString(statics,this->arg.integer)->getTerminatedBuffer();
		break;
	case Instruction::PUSH_FUNCTION:
		DebugOut(L"Instruction") << "PUSH_FUNCTION" << " " << this->arg.integer;
		break;
	case Instruction::PUSH_NULL:
		DebugOut(L"Instruction") << "PUSH_NULL";
		break;
	case Instruction::PUSH_DEFAULT:
		DebugOut(L"Instruction") << "PUSH_DEFAULT";
		break;
	case Instruction::INDEX:
		DebugOut(L"Instruction") << "INDEX";
		break;
	case Instruction::PROPERTY:
		DebugOut(L"Instruction") << "PROPERTY";
		break;
	case Instruction::POP:
		DebugOut(L"Instruction") << "POP";
		break;
	case Instruction::MAKE_GLOBAL_CONST:
		DebugOut(L"Instruction") << "MAKE_GLOBAL_CONST" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::MAKE_LOCAL_CONST:
		DebugOut(L"Instruction") << "MAKE_LOCAL_CONST" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::MAKE_NEAREST_CONST:
		DebugOut(L"Instruction") << "MAKE_NEAREST_CONST" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::ASSIGN_GLOBAL:
		DebugOut(L"Instruction") << "ASSIGN_GLOBAL" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::ASSIGN_LOCAL:
		DebugOut(L"Instruction") << "ASSIGN_LOCAL" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::ASSIGN_NEAREST:
		DebugOut(L"Instruction") << "ASSIGN_NEAREST" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::ASSIGN_INDEX:
		DebugOut(L"Instruction") << "ASSIGN_INDEX";
		break;
	case Instruction::ASSIGN_PROPERTY:
		DebugOut(L"Instruction") << "ASSIGN_PROPERTY" << " " << getName(statics,this->arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::JUMP_LONG_ABSOLUTE:
		DebugOut(L"Instruction") << "JUMP_LONG_ABSOLUTE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE:
		DebugOut(L"Instruction") << "JUMP_SHORT_ABSOLUTE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_RELATIVE:
		DebugOut(L"Instruction") << "JUMP_LONG_RELATIVE"  << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_RELATIVE:
		DebugOut(L"Instruction") << "JUMP_SHORT_RELATIVE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_ABSOLUTE_IF_TRUE:
		DebugOut(L"Instruction") << "JUMP_LONG_ABSOLUTE_IF_TRUE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE_IF_TRUE:
		DebugOut(L"Instruction") << "JUMP_SHORT_ABSOLUTE_IF_TRUE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_RELATIVE_IF_TRUE:
		DebugOut(L"Instruction") << "JUMP_LONG_RELATIVE_IF_TRUE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_RELATIVE_IF_TRUE:
		DebugOut(L"Instruction") << "JUMP_SHORT_RELATIVE_IF_TRUE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_ABSOLUTE_IF_FALSE:
		DebugOut(L"Instruction") << "JUMP_LONG_ABSOLUTE_IF_TRUE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_ABSOLUTE_IF_FALSE:
		DebugOut(L"Instruction") << "JUMP_SHORT_ABSOLUTE_IF_FALSE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::JUMP_LONG_RELATIVE_IF_FALSE:
		DebugOut(L"Instruction") << "JUMP_LONG_RELATIVE_IF_FALSE" << " " << this->arg.integer;
		break;
	case Instruction::JUMP_SHORT_RELATIVE_IF_FALSE:
		DebugOut(L"Instruction") << "JUMP_SHORT_RELATIVE_IF_FALSE" << " " <<  (int)this->arg.byte;
		break;
	case Instruction::NEGATION:
		DebugOut(L"Instruction") << "NEGATION";
		break;
	case Instruction::CALL_FUNCTION:
		DebugOut(L"Instruction") << "CALL_FUNCTION" << " " << (int)this->arg.byte;
		break;
	case Instruction::SWAP_TOP:
		DebugOut(L"Instruction") << "SWAP_TOP";
		break;
	case Instruction::DOUBLE_TOP:
		DebugOut(L"Instruction") << "DOUBLE_TOP";
		break;
	case Instruction::RET:
		DebugOut(L"Instruction") << "RET";
		break;
	case Instruction::PUSH_CATCH_BLOCK:
		DebugOut(L"Instruction") << "PUSH_CATCH_BLOCK";
		break;
	case Instruction::PUSH_FINALLY_BLOCK:
		DebugOut(L"Instruction") << "PUSH_FINALLY_BLOCK";
		break;
	case Instruction::PUSH_CURRENT_EXCEPTION:
		DebugOut(L"Instruction") << "PUSH_CURRENT_EXCEPTION";
		break;
	case Instruction::THROW_EXCEPTION:
		DebugOut(L"Instruction") << "THROW_EXCEPTION";
		break;
	case Instruction::EXIT_FINALLY:
		DebugOut(L"Instruction") << "EXIT_FINALLY";
		break;
	case Instruction::BOOLEAN_NOT:
		DebugOut(L"Instruction") << "BOOLEAN_NOT";
		break;
	case Instruction::BOOLEAN_OR:
		DebugOut(L"Instruction") << "BOOLEAN_OR";
		break;
	case Instruction::BOOLEAN_AND:
		DebugOut(L"Instruction") << "BOOLEAN_AND";
		break;
	case Instruction::ADDITION:
		DebugOut(L"Instruction") << "ADDITION";
		break;
	case Instruction::SUBTRACTION:
		DebugOut(L"Instruction") << "SUBTRACTION";
		break;
	case Instruction::MULTIPLICATION:
		DebugOut(L"Instruction") << "MULTIPLICATION";
		break;
	case Instruction::DIVISION:
		DebugOut(L"Instruction") << "DIVISION";
		break;
	case Instruction::GREATER:
		DebugOut(L"Instruction") << "GREATER";
		break;
	case Instruction::GREATER_EQUAL:
		DebugOut(L"Instruction") << "GREATER_EQUAL";
		break;
	case Instruction::LESSER:
		DebugOut(L"Instruction") << "LESSER";
		break;
	case Instruction::LESSER_EQUAL:
		DebugOut(L"Instruction") << "LESSER_EQUAL";
		break;
	case Instruction::EQUAL:
		DebugOut(L"Instruction") << "EQUAL";
		break;
	case Instruction::STRONG_STRING_EQUAL:
		DebugOut(L"Instruction") << "STRONG_STRING_EQUAL";
		break;
	case Instruction::EXACTLY_EQUAL:
		DebugOut(L"Instruction") << "EXACTLY_EQUAL";
		break;
	case Instruction::NOT_EQUAL:
		DebugOut(L"Instruction") << "NOT_EQUAL";
		break;
	case Instruction::CONCAT:
		DebugOut(L"Instruction") << "CONCAT";
		break;
	case Instruction::POW:
		DebugOut(L"Instruction") << "POW";
		break;
	case Instruction::TERMINATE:
		DebugOut(L"Instruction") << "TERMINATE";
		break;
	case Instruction::BUILD_LIST:
		DebugOut(L"Instruction") << "BUILD_LIST "<<arg.integer;
		break;
	case Instruction::BUILD_MAP:
		DebugOut(L"Instruction") << "BUILD_MAP "<<arg.integer;
		break;
	case Instruction::CREATE_MULTIDIM_LIST:
		DebugOut(L"Instruction") << "CREATE_MULTIDIM_LIST " << (int)arg.byte;
		break;
	case Instruction::REDIM_MULTIDIM_LIST:
		DebugOut(L"Instruction") << "REDIM_MULTIDIM_LIST " << (int)arg.byte;
		break;
	case Instruction::PUSH_MACRO:
		DebugOut(L"Instruction") << "PUSH_MACRO " << getMacro(statics,arg.integer)->getTerminatedBuffer();
		break;
	case Instruction::DOUBLE_TOP_TWO:
		DebugOut(L"Instruction") << "DOUBLE_TOP_TWO";
		break;
	case Instruction::PUSH_LOOP_BLOCK:
		DebugOut(L"Instruction") << "PUSH_LOOP_BLOCK " << arg.integerPair.i1 << " " << arg.integerPair.i2;
		break;
	case Instruction::POP_BLOCK:
		DebugOut(L"Instruction") << "POP_BLOCK";
		break;
	case Instruction::CONTINUE_LOOP:
		DebugOut(L"Instruction") << "CONTINUE_LOOP " << (int)arg.byte;
		break;
	case Instruction::BREAK_LOOP:
		DebugOut(L"Instruction") << "BREAK_LOOP " << (int)arg.byte;
		break;
	case Instruction::CREATE_ARGUMENT:
		DebugOut(L"Instruction") << "CREATE_ARGUMENT " << getName(statics,arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::CREATE_BYREF_ARGUMENT:
		DebugOut(L"Instruction") << "CREATE_BYREF_ARGUMENT " << getName(statics,arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::LOAD_ARGUMENTS:
		DebugOut(L"Instruction") << "LOAD_ARGUMENTS " << (int)arg.bytePair.b1 << " " << (int)arg.bytePair.b2;
		break;
	case Instruction::PUSH_GENERAL_BLOCK:
		DebugOut(L"Instruction") << "PUSH_GENERAL_BLOCK";
		break;
	case Instruction::CREATE_CLOSURE_NAME:
		DebugOut(L"Instruction") << "CREATE_CLOSURE_NAME " << getName(statics,arg.identifier)->getTerminatedBuffer();
		break;
	case Instruction::GET_ITERATOR:
		DebugOut(L"Instruction") << "GET_ITERATOR";
		break;
	case Instruction::ITERATOR_HAS_MORE:
		DebugOut(L"Instruction") << "ITERATOR_HAS_MORE";
		break;
	case Instruction::ITERATOR_NEXT:
		DebugOut(L"Instruction") << "ITERATOR_NEXT";
		break;
	default:
		DebugOut(L"Instruction") << "No pretty print available for instruction.";
		break;
	}
}