#include "GraphicsAndSoundFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include <Windows.h>
#include <functional>
#include "..\AI5Runtime\CallInfo.h"
using namespace std::placeholders;

GraphicsAndSoundFunctions::GraphicsAndSoundFunctions(void)
{
}


GraphicsAndSoundFunctions::~GraphicsAndSoundFunctions(void)
{
}


void GraphicsAndSoundFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<GraphicsAndSoundFunctions> instance(new GraphicsAndSoundFunctions);

	machine->addBuiltInFunction(L"beep",std::bind(&beep,instance,_1));

}

Variant* GraphicsAndSoundFunctions::beep(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,2);

	int freq = callInfo->getInt32Arg(0,500);
	int duration =  callInfo->getInt32Arg(1,1000);

	Beep(freq,duration);

	return new Integer32Variant(1);
}