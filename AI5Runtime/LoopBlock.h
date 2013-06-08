#pragma once
#include "block.h"
#include "PooledObject.h"
class LoopBlock :
	public Block, PooledObject<LoopBlock>
{
public:
	
	~LoopBlock(void);
	
	friend class PooledObject<LoopBlock>;
private:
	LoopBlock(void);
	static const int POOL_SIZE = 1024;
};

