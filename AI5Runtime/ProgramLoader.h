#pragma once
#include "StackMachine.h"
#include <memory>

// Factory class for creating a fully setup StackMachine from a binary file.
class ProgramLoader
{
public:
	static std::shared_ptr<StackMachine> LoadFromFile(const std::string&filename);
private:
	ProgramLoader(void);
	~ProgramLoader(void);
};

