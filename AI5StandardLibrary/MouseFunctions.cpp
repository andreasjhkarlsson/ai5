#include "MouseFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include "..\AI5Runtime\ListVariant.h"
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

VariantReference<> MouseFunctions::mouseMove(CallInfo* callInfo)
{
	callInfo->validateArgCount(2,3);

	int x = callInfo->getInt32Arg(0);
	int y = callInfo->getInt32Arg(1);
	int speed = callInfo->getInt32Arg(2,-1);

	long returnValue = AU3_MouseMove(x,y,speed);

	return returnValue;
}


VariantReference<> MouseFunctions::mouseDown(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,1);
	AU3_MouseDown(callInfo->getStringArg(0,L"left")->getTerminatedBuffer());
	return nullptr;
}
VariantReference<> MouseFunctions::mouseUp(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,1);
	AU3_MouseUp(callInfo->getStringArg(0,L"left")->getTerminatedBuffer());
	return nullptr;
}
VariantReference<> MouseFunctions::mouseClick(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,5);
	long ret = AU3_MouseClick(callInfo->getStringArg(0)->getTerminatedBuffer(),
		callInfo->getInt32Arg(1,AU3_INTDEFAULT),
		callInfo->getInt32Arg(2,AU3_INTDEFAULT),
		callInfo->getInt32Arg(3,1),
		callInfo->getInt32Arg(4,-1));
	return ret;
}
VariantReference<> MouseFunctions::mouseClickDrag(CallInfo* callInfo)
{
	callInfo->validateArgCount(5,6);
	long ret = AU3_MouseClickDrag(callInfo->getStringArg(0)->getTerminatedBuffer(),
		callInfo->getInt32Arg(1),
		callInfo->getInt32Arg(2),
		callInfo->getInt32Arg(3),
		callInfo->getInt32Arg(4),
		callInfo->getInt32Arg(5,-1));
	return ret;

}
VariantReference<> MouseFunctions::mouseGetPos(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,1);

	int mode = callInfo->getInt32Arg(0,-1);


	switch(mode)
	{
	case 0:
		return AU3_MouseGetPosX();
	case 1:
		return AU3_MouseGetPosY();
	case -1:
		{
		ListVariant* retArr = ListVariant::Create();
		VariantReference<> xEl = AU3_MouseGetPosX();
		VariantReference<> yEl = AU3_MouseGetPosY();
		retArr->addElement(xEl);
		retArr->addElement(yEl);
		return retArr;
		}
	default:
		return nullptr; // TODO throw.
	}

}

VariantReference<> MouseFunctions::mouseWheel(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);

	AU3_MouseWheel(callInfo->getStringArg(0)->getTerminatedBuffer(),callInfo->getInt32Arg(1,1));

	return nullptr;
}
VariantReference<> MouseFunctions::mouseGetCursor(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,0);
	return AU3_MouseGetCursor();
}



void MouseFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<MouseFunctions> instance(new MouseFunctions);

	machine->addBuiltInFunction(L"mousemove",std::bind(&mouseMove,instance,_1));
	machine->addBuiltInFunction(L"mouseclick",std::bind(&mouseClick,instance,_1));
	machine->addBuiltInFunction(L"mouseup",std::bind(&mouseUp,instance,_1));
	machine->addBuiltInFunction(L"mousedown",std::bind(&mouseDown,instance,_1));
	machine->addBuiltInFunction(L"mousegetcursor",std::bind(&mouseGetCursor,instance,_1));
	machine->addBuiltInFunction(L"mouseclickdrag",std::bind(&mouseClickDrag,instance,_1));
	machine->addBuiltInFunction(L"mousegetpos",std::bind(&mouseGetPos,instance,_1));
	machine->addBuiltInFunction(L"mousewheel",std::bind(&mouseWheel,instance,_1));

}