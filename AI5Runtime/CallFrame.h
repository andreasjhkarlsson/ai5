#pragma once
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"
#include "PooledObject.h"

class CallFrame: public Block, public PooledObject<CallFrame> 
{
public:
	
	~CallFrame(void);
	void setup(StackMachine* machine,int returnAddress,int numberOfArguments,CallFrame* parentFrame);

	Scope* getScope();
	virtual void leave(StackMachine*);
	void recycleInstance();
	int getReturnAddress();
	CallFrame* getParentFrame();
	
	friend class PooledObject<CallFrame>;
private:
	int returnAddress;
	int numberOfArguments;
	int stackPosition;
	CallFrame* parentFrame;
	Scope scope;
	CallFrame();
	static const int POOL_SIZE = 1024;
	
};

