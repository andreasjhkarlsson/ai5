#pragma once

class StackMachine;


typedef unsigned char BLOCK_TYPE;

class Block
{
public:
	Block(BLOCK_TYPE type);
	virtual void leave(StackMachine*)=0;
protected:
	void unwindStack(StackMachine*,int stackPosition);
private:
	BLOCK_TYPE type;
public:
	static const BLOCK_TYPE CALL_BLOCK = 0;
	static const BLOCK_TYPE LOOP_BLOCK = 1;
};