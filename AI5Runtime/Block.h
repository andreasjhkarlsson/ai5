#pragma once

class StackMachine;

class Block
{
public:
	Block(int stackRestorePosition);
	virtual void leave(StackMachine*);
private:
	int stackRestorePoint;

};