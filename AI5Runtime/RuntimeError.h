#pragma once
#include <string>
#include "types.h"

// This exception may be thrown at any time inside StackMachine::start
class RuntimeError
{
public:
	RuntimeError(const std::wstring& message): message(message)
	{

	}
	RuntimeError()
	{

	}
	const std::wstring& getMessage() const
	{
		return message;
	}
protected:
	void setMessage(const std::wstring& message)
	{
		this->message = message;
	}
private:
	std::wstring message;
};

class InvalidArgumentCountError: RuntimeError
{
public:
	InvalidArgumentCountError(int got,int required,int max)
	{
		if(got < required)
		{
			this->setMessage(std::wstring(L"Too few arguments to function call. Got ")+
			int2string(got)+L" expected "+int2string(required)+L".");
		}
		else
		{
			this->setMessage(std::wstring(L"Too many arguments to function call. Got ")+int2string(got)+L" max is "+int2string(max)+L".");
		}
	}
};