#pragma once
#include "block.h"
#include "PooledObject.h"

// This block primarely makes sure that the stack is balanced when a loop is exited mid
// execution (exitloop, continueloop).
// It also stores address of beginning of loop for easy continueloop handling.
class LoopBlock :
	public Block,public PooledObject<LoopBlock>
{
public:
	~LoopBlock(void);
	friend class PooledObject<LoopBlock>;
	void leave(StackMachine* machine);
	void setup(StackMachine* machine,int continuePosition,int exitPosition);
	void recycleInstance();
	int getContinueAddress();
	int getExitAddress();
private:
	size_t stackPosition;
	int continuePosition;
	int exitPosition;
	LoopBlock(void);
	static const int POOL_SIZE = 1024;
};

