#include "MouseFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include <functional>
#include <memory>
#include <Windows.h>
#include "autoit3.h"
using namespace std::placeholders;

MouseFunctions::MouseFunctions(void)
{
}

MouseFunctions::~MouseFunctions(void)
{
}

Variant* MouseFunctions::mouseMove(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,3);

	int x = callInfo->getInt32Arg(0);
	int y = callInfo->getInt32Arg(1);
	int speed = callInfo->getInt32Arg(2,-1);

	long returnValue = AU3_MouseMove(x,y,speed);

	return new Integer32Variant(returnValue);
}

void MouseFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<MouseFunctions> instance(new MouseFunctions);

	machine->addBuiltInFunction(L"mousemove",std::bind(&mouseMove,instance,_1));
}