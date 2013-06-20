#pragma once
#include <vector>
#include "FastStack.h"
#include "Scope.h"
#include "Block.h"
#include "PooledObject.h"
#include "UserFunctionVariant.h"

struct Argument
{
	NameIdentifier identifier;
	bool isByref;
};

// Stores all information about a function call to a user defined function.
// Call setup function each time an instance is to be used.
class CallBlock: public Block, public PooledObject<CallBlock> 
{
public:
	
	~CallBlock(void);
	void setup(StackMachine* machine,int returnAddress,int calledNumberOfArguments,CallBlock* parentFrame,UserFunctionVariant* owner);

	Scope* getScope();
	virtual void leave(StackMachine*);
	void recycleInstance();
	int getReturnAddress();
	CallBlock* getParentFrame();
	void addArgument(const Argument& argument);
	void loadArguments(StackMachine* machine,int total,int required);
	void addClosedName(StackMachine* machine,NameIdentifier nameIdentifier);
	void addClosure(StackMachine* machine,UserFunctionVariant* closure);
	
	friend class PooledObject<CallBlock>;
private:
	void processClosures(StackMachine*);
	int returnAddress;
	int calledNumberOfArguments;
	size_t stackPosition;
	CallBlock* parentFrame;
	UserFunctionVariant* owner;
	Scope* scope;
	Scope* closureScope;
	CallBlock();
	static const int POOL_SIZE = 1024;
	std::vector<Argument> arguments;
	std::vector<UserFunctionVariant*> closures;
	std::vector<NameIdentifier> closedNames;
	
};

