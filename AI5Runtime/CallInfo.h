#pragma once
#include "Variant.h"
class HandleVariant;
class StackMachineThread;
class CallInfo
{
public:
	CallInfo(StackMachineThread*);
	~CallInfo(void);
	void addArg(Variant*);
	Variant* getArg(int index);
	int getArgCount();
	void validateArgCount(int required,int max);
	double getFloatingArg(int index,double defaultValue=0);
	int getInt32Arg(int index,int defaultValue=0);
	__int64 getInt64Arg(int index,__int64 defaultValue=0);
	bool getBoolArg(int index,bool defaultValue=false);
	shared_string getStringArg(int index,const wchar_t* defaultValue=L"");
	HandleVariant* getHandleArg(int index);
	void setError(int error);
	void setExtended(int extended);
	static const int MAX_ARGS = 64;
private:
	Variant* args[MAX_ARGS];
	int argCount;
	StackMachineThread* machine;
};

