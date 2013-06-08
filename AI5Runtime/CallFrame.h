#pragma once
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"
#include "PooledObject.h"

class CallFrame: public Block, public PooledObject<CallFrame> 
{
public:
	CallFrame();
	~CallFrame(void);
	void setup(StackMachine* machine,int numberOfArguments);

	Scope* getScope();
	virtual void leave(StackMachine*);
	static const int POOL_SIZE = 1024;
private:
	int returnAddress;
	Scope scope;
	int numberOfArguments;
};

