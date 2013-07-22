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
	callInfo->validateArgCount(0,1);

	GC::collect(callInfo->getBoolArg(0,false));
	
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
	callInfo->validateArgCount(1,2);

	VariantReference<ThreadContext> thread = machine->getThreadManager()->createThread(machine);
	thread->getVirtualThread()->setStartFunction(callInfo->getArg(0)->cast<UserFunctionVariant>());
	thread->setThreadName(callInfo->getStringArg(1));
	thread->start();

	return thread;
}

VariantReference<> SystemFunctions::joinThread(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	VariantReference<ThreadContext> thread = callInfo->getArg(0).cast<ThreadContext>();
	SAFE_REGION(
		return thread->join();
	);
}