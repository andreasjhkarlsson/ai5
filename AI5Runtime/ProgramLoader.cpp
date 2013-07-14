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

// Header structure of a binary file.
struct ProgramHeader
{
	unsigned int magic; // Something that identifies this file.
	unsigned short compiler;
	unsigned short target;
	unsigned int entry_instruction; // Which instruction should execution start at.
	unsigned int instructions_start; // Where in the file the first instruction can be found.
	unsigned int instructions_size; // How many instructions there are.
	unsigned int statics_start; // Where in the file is the statics stored?
	unsigned int statics_size; // How many statics are there.
};


// This method creates a fully prepared StackMachine from a file.
// This involves decoding all instructions, statics, headers and 
// setting the proper state of the machine.
std::shared_ptr<StackMachine> ProgramLoader::LoadFromFile(const std::string&filename)
{
	std::ifstream in;

	in.open(filename, std::ios::in | std::ios::binary);

	// File couldn't be read!
	if(!in.is_open())
	{
		throw ProgramLoadError(std::wstring(L"Could not open file for reading.")); 
	}

	// This little scheme gets the filesize without
	// involving the OS.
	std::streampos start = in.tellg();
	in.seekg(0, std::ios::end);
	std::streampos end = in.tellg();
	in.seekg(0, std::ios::beg);


	// Read header directly into a ProgramHeader struct.
	ProgramHeader header;
	in.read(reinterpret_cast<char*>(&header),sizeof(header));

	if(header.magic != 0xFEEFDEEF)
	{
		throw ProgramLoadError(L"File is corrupt or of wrong type.");
	}

	// Read all instructions at once!
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
		case Instruction::PUSH_DEFAULT					:
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
		case Instruction::STRONG_STRING_EQUAL			:
		case Instruction::NOT_EQUAL						:
		case Instruction::CONCAT						:
		case Instruction::POW							:
		case Instruction::TERMINATE						:
		case Instruction::NEGATION						:
		case Instruction::SWAP_TOP						:
		case Instruction::DOUBLE_TOP					:
		case Instruction::DOUBLE_TOP_TWO				:
		case Instruction::RET							:
		case Instruction::ASSIGN_INDEX					:
		case Instruction::POP_EXCEPTION_HANDLER			:
		case Instruction::RAISE_EXCEPTION				:
		case Instruction::POP_BLOCK						:
		case Instruction::EXACTLY_EQUAL					:
		case Instruction::PUSH_GENERAL_BLOCK			:
		case Instruction::GET_ITERATOR					:
		case Instruction::ITERATOR_HAS_MORE				:
		case Instruction::ITERATOR_NEXT					:
			instructions->push_back(Instruction::PTR(new Instruction(instructionBuffer[pos])));
			pos++;
			break;
		// int argument
		case Instruction::BUILD_LIST					:
		case Instruction::PUSH_STRING					:
		case Instruction::PUSH_FUNCTION					:
		case Instruction::PUSH_EXCEPTION_HANDLER		:
		case Instruction::JUMP_LONG_ABSOLUTE_IF_FALSE	:
		case Instruction::JUMP_LONG_ABSOLUTE_IF_TRUE	:
		case Instruction::JUMP_LONG_ABSOLUTE			:
		case Instruction::JUMP_LONG_RELATIVE_IF_TRUE	:
		case Instruction::JUMP_LONG_RELATIVE			:
		case Instruction::JUMP_LONG_RELATIVE_IF_FALSE	:
		case Instruction::PUSH_FLOATING					:
		case Instruction::PUSH_INTEGER32				:
		case Instruction::PUSH_INTEGER64				:
		case Instruction::PUSH_MACRO					:
		case Instruction::BUILD_MAP						:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.integer = *(int*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 5;
			break;
		// identifier argument.
		case Instruction::ASSIGN_GLOBAL					:
		case Instruction::ASSIGN_LOCAL					:
		case Instruction::ASSIGN_NEAREST				:			
		case Instruction::PUSH_NAME_VALUE				:
		case Instruction::PUSH_NAME						:
		case Instruction::ASSIGN_PROPERTY				:
		case Instruction::PROPERTY						:
		case Instruction::MAKE_GLOBAL_CONST				:
		case Instruction::MAKE_LOCAL_CONST				:
		case Instruction::MAKE_NEAREST_CONST			:
		case Instruction::CREATE_ARGUMENT				:
		case Instruction::CREATE_BYREF_ARGUMENT			:
		case Instruction::CREATE_CLOSURE_NAME			:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.identifier = *(NameIdentifier*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 13;
			break;
		// char argument.
		case Instruction::CREATE_MULTIDIM_LIST			:
		case Instruction::REDIM_MULTIDIM_LIST			:
		case Instruction::JUMP_SHORT_RELATIVE			:
		case Instruction::JUMP_SHORT_ABSOLUTE			:
		case Instruction::JUMP_SHORT_ABSOLUTE_IF_TRUE	:
		case Instruction::JUMP_SHORT_RELATIVE_IF_TRUE	:
		case Instruction::JUMP_SHORT_ABSOLUTE_IF_FALSE	:
		case Instruction::JUMP_SHORT_RELATIVE_IF_FALSE	:
		case Instruction::PUSH_BOOLEAN					:
		case Instruction::CALL_FUNCTION					:
		case Instruction::CONTINUE_LOOP					:
		case Instruction::BREAK_LOOP					:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			inst->arg.byte = *(char*)&instructionBuffer[pos+1];
			instructions->push_back(inst);
			pos += 2;
			break;
		// Two int arg.
		case Instruction::PUSH_LOOP_BLOCK				:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			memcpy(&inst->arg.integerPair,&instructionBuffer[pos+1],sizeof(int)*2);
			instructions->push_back(inst);
			pos += 9;
			break;
		// Two byte arg.
		case Instruction::LOAD_ARGUMENTS				:
			inst = Instruction::PTR(new Instruction(instructionBuffer[pos]));
			memcpy(&inst->arg.bytePair,&instructionBuffer[pos+1],sizeof(char)*2);
			instructions->push_back(inst);
			pos += 3;
			break;
		}
	}

	// Allocate statics and save it in a shared_ptr as someone else need
	// to deallocate it.
	auto statics = shared_ptr<vector<StaticData::PTR>>(new vector<StaticData::PTR>());
	
	// Seek to correct position in file for statics table.
	in.seekg(header.statics_start,std::ios::beg);

	// Read entire statics table at once!
	// If this turns out to be too big, change to memory mapped file instead.
	std::vector<unsigned char> staticsBuffer;
	staticsBuffer.resize(header.statics_size);
	in.read(reinterpret_cast<char*>(&staticsBuffer[0]),header.statics_size);

	// Loop through each static.
	// Since statics can have different lengths, let each decode routine
	// advance the index.
	for (size_t index = 0;index < header.statics_size;)
	{
		StaticData::PTR inst;
		switch(staticsBuffer[index])
		{
		case StaticData::NAME:
			{
				// Names are prefixed by 32 bit length.
				index++;
				unsigned int nameSize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				shared_string name;
				name = Encode::utf8_to_utf16((const char*)&staticsBuffer[index],nameSize);
				index += nameSize;
				statics->push_back(StaticData::PTR(new StaticName(name)));
				
			}
			break;
		case StaticData::STRING:
			{
				// Bad DRY from NAME here.. :P
				index++;
				unsigned int strsize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				shared_string str;
				str = Encode::utf8_to_utf16((const char*)&staticsBuffer[index],strsize);
				index += strsize;
				statics->push_back(StaticData::PTR(new StaticString(str)));
				
			}
			break;
		case StaticData::MACRO:
			{
				// Bad DRY from NAME here.. :P
				index++;
				unsigned int strsize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				shared_string str;
				str = Encode::utf8_to_utf16((const char*)&staticsBuffer[index],strsize);
				index += strsize;
				statics->push_back(StaticData::PTR(new StaticMacro(str)));
				
			}
			break;
		case StaticData::FLOATING:
			{
				// Floats are stored as string to avoid problems with
				// floating point binary standards.
				index++;
				unsigned int strsize = *(unsigned int*)&staticsBuffer[index];
				index += sizeof(unsigned int);
				double value = atof(std::string((const char*)&staticsBuffer[index],strsize).c_str());				
				index += strsize;
				statics->push_back(StaticData::PTR(new StaticFloating(value)));
			}
			break;
		case StaticData::INTEGER64:
			{
				index++;
				__int64 value = *(__int64*)&staticsBuffer[index];
				index += sizeof(__int64);

				statics->push_back(StaticData::PTR(new StaticInteger64(value)));
			}
			break;
		case StaticData::INTEGER32:
			{
				index++;
				int value = *(int*)&staticsBuffer[index];
				index += sizeof(int);
				statics->push_back(StaticData::PTR(new StaticInteger32(value)));
			}
			break;
		}

	}

	StackMachine* machine = new StackMachine(statics,instructions,header.entry_instruction);


	return shared_ptr<StackMachine>(machine);
}


ProgramLoadError::ProgramLoadError(const std::wstring& message): message(message)
{

}

const std::wstring& ProgramLoadError::getMessage() const
{
	return message;
}