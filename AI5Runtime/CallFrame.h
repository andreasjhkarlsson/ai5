#pragma once
#include <vector>
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"
#include "PooledObject.h"

struct Argument
{
	NameIdentifier identifier;
	bool isByref;
};

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
	void addArgument(const Argument& argument);
	void loadArguments(StackMachine* machine,int total,int required);
	
	friend class PooledObject<CallFrame>;
private:
	int returnAddress;
	int numberOfArguments;
	int stackPosition;
	CallFrame* parentFrame;
	Scope scope;
	CallFrame();
	static const int POOL_SIZE = 1024;
	std::vector<Argument> arguments;
	
};

