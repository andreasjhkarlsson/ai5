#pragma once
#include "block.h"
#include "PooledObject.h"
class CatchBlock : 
	public Block, public PooledObject<CatchBlock>
{
public:
	friend class PooledObject<CatchBlock>;
	
	void setup(StackMachineThread*,int address);
	virtual void leave(StackMachineThread*);
	virtual void recycleInstance();
	int getAddress();
	~CatchBlock(void);
private:
	size_t stackPos;
	int address;
	static const int POOL_SIZE = 32;
	CatchBlock();
};

