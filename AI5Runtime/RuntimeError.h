#pragma once
#include <string>
#include "types.h"

// This exception may be thrown at any time inside StackMachine::start
class RuntimeError
{
public:
	RuntimeError(const UnicodeString& message): message(message)
	{

	}
	RuntimeError()
	{

	}
	UnicodeString& getMessage()
	{
		return message;
	}
protected:
	void setMessage(const UnicodeString& message)
	{
		this->message = message;
	}
private:
	UnicodeString message;
};

class InvalidArgumentCountError: public RuntimeError
{
public:
	InvalidArgumentCountError(int got,int required,int max)
	{
		if(got < required)
		{
			this->setMessage(UnicodeString(L"Too few arguments to function call. Got ")+
			int2string(got)+L" expected "+int2string(required)+L".");
		}
		else
		{
			this->setMessage(UnicodeString(L"Too many arguments to function call. Got ")+int2string(got)+L" max is "+int2string(max)+L".");
		}
	}
};