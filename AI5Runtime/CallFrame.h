#pragma once
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"
#include "PooledObject.h"

class CallFrame: public Block, public PooledObject<CallFrame> 
{
public:
	
	~CallFrame(void);
	void setup(StackMachine* machine,int returnAddress,int numberOfArguments);

	Scope* getScope();
	virtual void leave(StackMachine*);
	
	friend class PooledObject<CallFrame>;
private:
	int returnAddress;
	int numberOfArguments;
	int stackPosition;
	Scope scope;
	CallFrame();
	static const int POOL_SIZE = 1024;
	
};

