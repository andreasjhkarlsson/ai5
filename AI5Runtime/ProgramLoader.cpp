#include "ProgramLoader.h"
#include <Windows.h>
#include <fstream>
#include "Instruction.h"
#include "encode.h"
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


		case Instruction::PUSH_STRING:
		case Instruction::PUSH_FUNCTION:
		
		
		case Instruction::PUSH_EXCEPTION_HANDLER		:
		case Instruction::JUMP_LONG_ABSOLUTE_IF_FALSE	:
		case Instruction::JUMP_LONG_ABSOLUTE_IF_TRUE	:
		case Instruction::JUMP_LONG_ABSOLUTE			:
		case Instruction::JUMP_LONG_RELATIVE_IF_TRUE	:
		case Instruction::JUMP_LONG_RELATIVE			:
		case Instruction::JUMP_LONG_RELATIVE_IF_FALSE	:
		case Instruction::PUSH_FLOATING:
		case Instruction::PUSH_INTEGER:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.integer = *(int*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 5;
			break;
		case Instruction::ASSIGN_GLOBAL					:
		case Instruction::ASSIGN_LOCAL					:
		case Instruction::ASSIGN_NEAREST				:			
		case Instruction::PUSH_NAME:
		case Instruction::ASSIGN_PROPERTY				:
		case Instruction::PROPERTY						:
		case Instruction::ASSIGN_GLOBAL_CONST:
		case Instruction::ASSIGN_LOCAL_CONST:
		case Instruction::ASSIGN_NEAREST_CONST:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.identifier = *(NameIdentifier*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 13;
			break;
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
		}
	}

	auto statics = shared_ptr<vector<StaticData::PTR>>(new vector<StaticData::PTR>());
	std::vector<unsigned char> staticsBuffer;
	staticsBuffer.resize(header.statics_size);
	in.seekg(header.statics_start,std::ios::beg);
	in.read(reinterpret_cast<char*>(&staticsBuffer[0]),header.statics_size);

	for (size_t index = 0;index < header.statics_size;)
	{
		StaticData::PTR inst;
		switch(staticsBuffer[index])
		{
		case StaticData::NAME:
			{
				index++;
				unsigned int nameSize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				std::wstring name = L"";
				if(nameSize > 0)
					name = utf8_to_utf16((const char*)&staticsBuffer[index],nameSize);
				index += nameSize;
				statics->push_back(StaticData::PTR(new StaticName(name)));
				
			}
			break;
		case StaticData::STRING:
			{
				index++;
				unsigned int strsize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				std::wstring str = L"";
				if(strsize > 0)
					str = utf8_to_utf16((const char*)&staticsBuffer[index],strsize);
				index += strsize;
				statics->push_back(StaticData::PTR(new StaticString(str)));
				
			}
			break;
		case StaticData::FLOATING:
			{
				index++;
				unsigned int strsize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				double value = atof(std::string((const char*)&staticsBuffer[index],strsize).c_str());				
				index += strsize;
				statics->push_back(StaticData::PTR(new StaticFloating(value)));
			}
			break;
		case StaticData::INTEGER:
			{
				index++;
				__int64 value = *(__int64*)&staticsBuffer[index];
				index += sizeof(__int64);

				statics->push_back(StaticData::PTR(new StaticInteger(value)));
			}
			break;
		}
	}


	StackMachine* machine = new StackMachine(statics,instructions);
	machine->jumpAbsolute(header.entry_instruction);

	return shared_ptr<StackMachine>(machine);
}