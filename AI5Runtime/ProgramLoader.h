#pragma once
#include "StackMachineThread.h"
#include <memory>
#include <string>

class ProgramLoadError
{
public:
	ProgramLoadError(const std::wstring& message);
	const std::wstring& getMessage() const;
private:
	const std::wstring message;
};

// Factory class for creating a fully setup StackMachine from a binary file.
class ProgramLoader
{
public:
	static std::shared_ptr<StackMachineThread> LoadFromFile(const std::string&filename);
private:
	ProgramLoader(void);
	~ProgramLoader(void);
};

