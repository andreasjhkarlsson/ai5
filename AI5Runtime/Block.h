#pragma once

class StackMachine;

class Block
{
public:
	Block(int stackRestorePosition);
	virtual void leave(StackMachine*);
protected:
	void setStackReturnPoint(int point)
	{
		stackRestorePoint = point;
	}
private:
	int stackRestorePoint;

};