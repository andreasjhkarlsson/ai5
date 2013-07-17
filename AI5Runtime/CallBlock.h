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
	void setup(StackMachineThread* machine,int returnAddress,int calledNumberOfArguments,CallBlock* parentFrame,VariantReference<UserFunctionVariant>& owner);

	VariantReference<Scope>& getScope();
	virtual void leave(StackMachineThread*);
	void recycleInstance();
	int getReturnAddress();
	CallBlock* getParentFrame();
	void addArgument(const Argument& argument);
	void loadArguments(StackMachineThread* machine,int total,int required);
	void addClosedName(StackMachineThread* machine,NameIdentifier nameIdentifier);
	void addClosure(StackMachineThread* machine,const VariantReference<UserFunctionVariant>& closure);
	
	friend class PooledObject<CallBlock>;
private:
	void processClosures(StackMachineThread*);
	int returnAddress;
	int calledNumberOfArguments;
	size_t stackPosition;
	CallBlock* parentFrame;
	VariantReference<UserFunctionVariant> owner;
	VariantReference<Scope> scope;
	VariantReference<Scope> closureScope;
	CallBlock();
	static const int POOL_SIZE = 1024;
	std::vector<Argument> arguments;
	std::vector<VariantReference<UserFunctionVariant>> closures;
	std::vector<NameIdentifier> closedNames;
	
};

