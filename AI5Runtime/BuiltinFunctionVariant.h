#pragma once
#include <string>
#include <vector>
#include <functional>
#include "FunctionVariant.h"
class StackMachine;

typedef Variant* (*BuiltinFunctionPointer)(Variant** args,int argsSize);
typedef std::function<Variant*(Variant**,int)> BuiltinFunction;

class BuiltinFunctionVariant :
	public FunctionVariant
{
public:
	BuiltinFunctionVariant(const std::wstring &name,BuiltinFunction func);
	~BuiltinFunctionVariant(void);
	void call(StackMachine* machine,int numberOfArguments);
	std::wostream& format(std::wostream& stream) const;
	virtual bool equal(Variant*);
private:
	BuiltinFunction func;
	std::wstring name;
	std::vector<Variant*> passedArgs;
	static const int MAX_ARGS = 128;
};

