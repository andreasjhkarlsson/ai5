#pragma once
#include "block.h"
#include "PooledObject.h"
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
	int stackPosition;
	int continuePosition;
	int exitPosition;
	LoopBlock(void);
	static const int POOL_SIZE = 1024;
};

