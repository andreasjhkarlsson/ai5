#include "ProgramLoader.h"
#include <Windows.h>
#include <fstream>
#include "Instruction.h"

#include <memory>
using std::shared_ptr;
using std::vector;

ProgramLoader::ProgramLoader(void)
{
}


ProgramLoader::~ProgramLoader(void)
{
}



std::shared_ptr<StackMachine> ProgramLoader::LoadFromFile(const std::string&filename)
{
	std::ifstream in;

	in.open(filename, std::ios::in | std::ios::binary);

	if(!in.is_open())
	{
		return nullptr;
	}
	// get the starting position
	std::streampos start = in.tellg();

	// go to the end
	in.seekg(0, std::ios::end);

	// get the ending position
	std::streampos end = in.tellg();

	// go back to the start
	in.seekg(0, std::ios::beg);

	// create a vector to hold the data that
	// is resized to the total size of the file    
	std::vector<char> contents;
	contents.resize(static_cast<size_t>(end - start));

	// read it in
	//	in.read(&contents[0], contents.size());

	ProgramHeader header;
	in.read(reinterpret_cast<char*>(&header),sizeof(header));


	std::vector<unsigned char> instructionBuffer;
	instructionBuffer.resize(header.instructions_size);
	in.seekg(header.instructions_start,std::ios::beg);
	in.read(reinterpret_cast<char*>(&instructionBuffer[0]),header.instructions_size);

	auto instructions = shared_ptr<vector<Instruction::PTR>>(new vector<Instruction::PTR>);
	for (unsigned int pos = 0;pos < header.instructions_size;)
	{
		Instruction::PTR inst;
		switch(instructionBuffer[pos])
		{
			// No arguments
		case Instruction::POP							:
		case Instruction::NOOP							:
		case Instruction::PUSH_NULL						:
		case Instruction::INDEX							:
		case Instruction::BOOLEAN_NOT					:
		case Instruction::BOOLEAN_OR					:
		case Instruction::BOOLEAN_AND					:
		case Instruction::ADDITION						:
		case Instruction::SUBTRACTION					:
		case Instruction::MULTIPLICATION				:
		case Instruction::DIVISION						:
		case Instruction::GREATER						:
		case Instruction::GREATER_EQUAL					:
		case Instruction::LESSER						:
		case Instruction::LESSER_EQUAL					:
		case Instruction::EQUAL							:
		case Instruction::STRONG_EQUAL					:
		case Instruction::NOT_EQUAL						:
		case Instruction::CONCAT						:
		case Instruction::POW							:
		case Instruction::TERMINATE						:
		case Instruction::NEGATION						:
		case Instruction::SWAP_TOP						:
		case Instruction::DOUBLE_TOP					:
		case Instruction::RET							:
		case Instruction::ASSIGN_INDEX					:
		case Instruction::POP_EXCEPTION_HANDLER			:
		case Instruction::RAISE_EXCEPTION				:
			instructions->push_back(Instruction::PTR(new Instruction(instructionBuffer[pos])));
			pos++;
			break;
			// int argument
		case Instruction::CREATE_GLOBAL					:
		case Instruction::CREATE_LOCAL					:
		case Instruction::ASSIGN_NAME					:
		case Instruction::PUSH_NAME:
		case Instruction::PUSH_STRING:
		case Instruction::PUSH_FUNCTION:
		case Instruction::PROPERTY						:
		case Instruction::ASSIGN_PROPERTY				:
		case Instruction::PUSH_EXCEPTION_HANDLER		:
		case Instruction::JUMP_LONG_ABSOLUTE_IF_FALSE	:
		case Instruction::JUMP_LONG_ABSOLUTE_IF_TRUE	:
		case Instruction::JUMP_LONG_ABSOLUTE			:
		case Instruction::JUMP_LONG_RELATIVE_IF_TRUE	:
		case Instruction::JUMP_LONG_RELATIVE			:
		case Instruction::JUMP_LONG_RELATIVE_IF_FALSE	:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.integer = *(int*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 5;
			break;
			// int64 argument
		case Instruction::PUSH_LARGE_INTEGER:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.int64 = *(int*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 9;
			break;
			// char argument
		case Instruction::PUSH_SMALL_INTEGER:
		case Instruction::JUMP_SHORT_RELATIVE			:
		case Instruction::JUMP_SHORT_ABSOLUTE			:
		case Instruction::JUMP_SHORT_ABSOLUTE_IF_TRUE	:
		case Instruction::JUMP_SHORT_RELATIVE_IF_TRUE	:
		case Instruction::JUMP_SHORT_ABSOLUTE_IF_FALSE	:
		case Instruction::JUMP_SHORT_RELATIVE_IF_FALSE	:
		case Instruction::PUSH_BOOLEAN:
		case Instruction::CALL_FUNCTION:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.byte = *(char*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 2;
			break;
			// double argument
		case Instruction::PUSH_FLOATING:
	//		instructions->push_back(Instruction::PTR(new Instruction(instructionBuffer[pos])));
			pos += 9;
			break;
		}
	}

	auto statics = shared_ptr<vector<StaticData::PTR>>(new vector<StaticData::PTR>());
	std::vector<unsigned char> staticsBuffer;
	staticsBuffer.resize(header.statics_size);
	in.seekg(header.statics_start,std::ios::beg);
	in.read(reinterpret_cast<char*>(&staticsBuffer[0]),header.statics_size);

	for (unsigned int index = 0;index < header.statics_size;)
	{
		StaticData::PTR inst;
		switch(staticsBuffer[index])
		{
		case StaticData::NAME:
			index++;
			size_t strsize = *(unsigned int*)&staticsBuffer[index];
			index += 4;
			std::string str((const char*)&staticsBuffer[index],strsize);
			index += strsize;
			statics->push_back(StaticData::PTR(new StaticName(str)));
			break;

		}
	}


	StackMachine* machine = new StackMachine(statics,instructions);
	machine->jumpAbsolute(header.entry_instruction);




	return shared_ptr<StackMachine>(machine);
}