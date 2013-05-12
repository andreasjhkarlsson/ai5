#pragma once
#include "StackMachine.h"
#include <memory>
class ProgramLoader
{
public:
	static std::shared_ptr<StackMachine> LoadFromFile(const std::string&filename);
private:
	ProgramLoader(void);
	~ProgramLoader(void);
};

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

