#pragma once
#include <string>


// This exception may be thrown at any time inside StackMachine::start
class RuntimeError
{
public:
	RuntimeError(const std::wstring& message): message(message)
	{

	}
	const std::wstring& getMessage() const
	{
		return message;
	}
private:
	const std::wstring message;
};