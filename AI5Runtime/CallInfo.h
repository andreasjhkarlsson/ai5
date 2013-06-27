#pragma once
#include "Variant.h"
class HandleVariant;
class StackMachine;
class CallInfo
{
public:
	CallInfo(StackMachine*);
	~CallInfo(void);
	void addArg(Variant*);
	Variant* getArg(int index);
	int getArgCount();
	void validateArgCount(int required,int max);
	double getFloatingArg(int index,double defaultValue=0);
	int getInt32Arg(int index,int defaultValue=0);
	__int64 getInt64Arg(int index,__int64 defaultValue=0);
	bool getBoolArg(int index,bool defaultValue=false);
	shared_string getStringArg(int index);
	HandleVariant* getHandleArg(int index);
	void setError(int error,int extended=0);
	void setError(Variant* error,Variant* extended);
	static const int MAX_ARGS = 64;
private:
	Variant* args[MAX_ARGS];
	int argCount;
	StackMachine* machine;
};

