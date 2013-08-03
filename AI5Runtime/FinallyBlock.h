#pragma once
#include "block.h"
#include "PooledObject.h"
#include "VariantReference.h"
class FinallyBlock :
	public Block, public PooledObject<FinallyBlock>
{
public:
	struct ReturnInfo
	{
		enum ReturnAction
		{
			THROW_EXCEPTION,
			JUMP_TO,
			RETURN_FROM_FUNC,
			EXITLOOP,
			CONTINUELOOP
		} action;
		int returnAddress;
		int level;
		VariantReference<> exceptionToThrow;
		VariantReference<> returnValue;
	};
	friend class PooledObject<FinallyBlock>;
	void setup(StackMachineThread* machine,int address);
	virtual void leave(StackMachineThread*);
	virtual void recycleInstance();
	void setRethrowAction(const VariantReference<> ex);
	void setFunctionReturnAction(const VariantReference<>& returnValue);
	void setJumpToAction(int address);
	void setContinueLoopAction(int level);
	void setExitLoopAction(int level);
	void execute(StackMachineThread*);
	const ReturnInfo& getReturnInfo();
	~FinallyBlock(void);
private:
	bool executing;
	size_t stackPos;
	FinallyBlock();
	static const int POOL_SIZE = 32;
	int address;
	ReturnInfo returnInfo;

};

