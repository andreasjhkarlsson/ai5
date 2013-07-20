#include "SystemFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include "..\AI5Runtime\gc.h"
#include <functional>
#include <memory>
using namespace std::placeholders;

SystemFunctions::SystemFunctions(StackMachine* machine): machine(machine)
{
}

SystemFunctions::~SystemFunctions(void)
{
}

VariantReference<> SystemFunctions::runGC(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,0);

	GC::collect(true);
	
	return nullptr;
}

void SystemFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<SystemFunctions> instance(new SystemFunctions(machine));

	machine->addBuiltInFunction(L"rungc",std::bind(&runGC,instance,_1));
	machine->addBuiltInFunction(L"startthread",std::bind(&startThread,instance,_1));
	machine->addBuiltInFunction(L"jointhread",std::bind(&joinThread,instance,_1));
}


VariantReference<> SystemFunctions::startThread(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	VariantReference<ThreadHandle> thread = machine->createThread();
	thread->getMachineThread()->startThread(callInfo->getArg(0)->cast<UserFunctionVariant>());

	return thread;
}

VariantReference<> SystemFunctions::joinThread(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	// Yiiihaaa! Crazy chain coming up!
	return callInfo->getArg(0).cast<HandleVariant>()->castHandle<ThreadHandle>()->getMachineThread()->join();
}