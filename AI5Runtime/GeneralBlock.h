#pragma once
#include "block.h"
#include "PooledObject.h"
class GeneralBlock :
	public Block, public PooledObject<GeneralBlock>
{
public:
	~GeneralBlock(void);
	friend class PooledObject<GeneralBlock>;
	void leave(StackMachine* machine);
	void setup(StackMachine* machine);
	void recycleInstance();
private:
	GeneralBlock(void);
	size_t stackPos;
	static const int POOL_SIZE = 1024;
};

