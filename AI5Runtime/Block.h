#pragma once

class StackMachineThread;


typedef unsigned char BLOCK_TYPE;

// A block is similar to scopes and exists for several reasons.
// To balance stack when exiting a piece of code unexpectedly (such as exception, return and exitloop)
// and to store data associated with a block (for example: return address for function call blocks).
class Block
{
public:
	virtual void leave(StackMachineThread*)=0;
	virtual void recycleInstance()=0;
	bool isCallBlock();
	bool isLoopBlock();
	BLOCK_TYPE getType();
protected:
	Block(BLOCK_TYPE type);
	void unwindStack(StackMachineThread*,size_t stackPosition);
private:
	BLOCK_TYPE type;
public:
	static const BLOCK_TYPE CALL_BLOCK = 0;
	static const BLOCK_TYPE LOOP_BLOCK = 1;
	static const BLOCK_TYPE GENERAL_BLOCK = 2;
};