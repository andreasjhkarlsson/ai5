#pragma once
#include <string>
#include "FunctionVariant.h"
class StackMachine;

typedef void (*BuiltinFunctionPointer)(StackMachine*);

class BuiltinFunctionVariant :
	public FunctionVariant
{
public:
	BuiltinFunctionVariant(const std::wstring &name,BuiltinFunctionPointer func);
	~BuiltinFunctionVariant(void);
	void call(StackMachine* machine,int numberOfArguments);
	std::wostream& format(std::wostream& stream);
	virtual bool equal(Variant*);
private:
	BuiltinFunctionPointer func;
	std::wstring name;
};

