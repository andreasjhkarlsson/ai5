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

// Stores all information about a function call to a user defined function.
// Call setup function each time an instance is to be used.
class CallFrame: public Block, public PooledObject<CallFrame> 
{
public:
	
	~CallFrame(void);
	void setup(StackMachine* machine,int returnAddress,int calledNumberOfArguments,CallFrame* parentFrame);

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
	int calledNumberOfArguments;
	size_t stackPosition;
	CallFrame* parentFrame;
	Scope* scope;
	CallFrame();
	static const int POOL_SIZE = 1024;
	std::vector<Argument> arguments;
	
};

