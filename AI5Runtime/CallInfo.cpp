#include "CallInfo.h"
#include "StackMachineThread.h"
#include "HandleVariant.h"
#include "RuntimeError.h"

CallInfo::CallInfo(StackMachineThread* machine): argCount(0),machine(machine)
{

}

void CallInfo::addArg(const VariantReference<>& arg)
{
	args[argCount++] = arg;
}
VariantReference<> CallInfo::getArg(int index)
{
	VariantReference<>& arg = args[index];
	if(arg.isNameType() || arg.isNameReferenceType())
	{
		return arg.cast<NameVariant>()->getValue();
	}
	return arg;
}
int CallInfo::getArgCount()
{
	return argCount;
}
void CallInfo::validateArgCount(int required,int max)
{
	if(argCount < required || argCount > max)
		throw InvalidArgumentCountError(argCount,required,max);

}

double CallInfo::getFloatingArg(int index,double defaultValue)
{
	if(index >= argCount)
		return defaultValue;
	return getArg(index).toFloating();
}

int CallInfo::getInt32Arg(int index,int defaultValue)
{
	if(index >= argCount)
		return defaultValue;
	return getArg(index).toInteger32();
}
__int64 CallInfo::getInt64Arg(int index,__int64 defaultValue)
{
	if(index >= argCount)
		return defaultValue;
	return getArg(index).toInteger64();
}
bool CallInfo::getBoolArg(int index,bool defaultValue)
{
	if(index >= argCount)
		return defaultValue;
	return getArg(index).toBoolean();
}
shared_string CallInfo::getStringArg(int index,const wchar_t* defaultValue)
{
	if(index >= argCount)
		return shared_string(new UnicodeString(defaultValue));
	return getArg(index).toString();
}

void CallInfo::setError(int error)
{
	machine->setErrorCode(error);
}


void CallInfo::setExtended(int extended)
{
	machine->setExtendedCode(extended);
}

CallInfo::~CallInfo(void)
{

}
