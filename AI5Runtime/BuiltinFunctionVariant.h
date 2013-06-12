#pragma once
#include <string>
#include <vector>
#include "FunctionVariant.h"
class StackMachine;

typedef Variant* (*BuiltinFunctionPointer)(Variant** args,int argsSize);

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
	std::vector<Variant*> passedArgs;
	static const int MAX_ARGS = 128;
};

