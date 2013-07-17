#pragma once
#include <string>
#include <vector>
#include <functional>
#include "FunctionVariant.h"
#include "CallInfo.h"
class StackMachineThread;

typedef VariantReference<>(*BuiltinFunctionPointer)(CallInfo*);
typedef std::function<VariantReference<>(CallInfo*)> BuiltinFunction;

class BuiltinFunctionVariant :
	public FunctionVariant
{
public:
	static const VARIANT_TYPE TYPE = NATIVE_FUNCTION;
	BuiltinFunctionVariant(const UnicodeString &name,BuiltinFunction func);
	~BuiltinFunctionVariant(void);
	void call(StackMachineThread* machine,int numberOfArguments);
	std::wostream& format(std::wostream& stream) const;
	virtual bool equal(Variant*);
private:
	BuiltinFunction func;
	UnicodeString name;
	std::vector<Variant*> passedArgs;
};

