#include "GraphicsAndSoundFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\Integer32Variant.h"
#include <Windows.h>
#include <functional>
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

	machine->addBuiltInFunction(L"beep",std::bind(&beep,instance,_1,_2));

}

Variant* GraphicsAndSoundFunctions::beep(Variant** args,int argsSize)
{
	validateArgCount(argsSize,0,2);

	int freq = 500;
	int duration = 1000;
	if(argsSize >= 1)
		freq = args[0]->toInteger32();
	if(argsSize >= 2)
		duration = args[1]->toInteger32();

	Beep(freq,duration);

	return new Integer32Variant(1);
}