#include "ThreadFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\CallInfo.h"
#include "..\AI5Runtime\gc.h"
#include <functional>
#include <memory>
using namespace std::placeholders;

ThreadFunctions::ThreadFunctions()
{

}

class MutexHandle: public HandleVariant
{
public:
	friend class GC;
	static const HANDLE_TYPE HTYPE = MUTEX_HANDLE;
	void lock()
	{
		mutex.lock();
	}
	void unlock()
	{
		mutex.unlock();
	}
	virtual bool isValid()const
	{
		return true;
	}
private:
	MutexHandle(): HandleVariant(MUTEX_HANDLE) {}
	std::recursive_mutex mutex;
};

VariantReference<> ThreadFunctions::createMutex(CallInfo* callInfo)
{
	return GC::alloc<MutexHandle>();
}
VariantReference<> ThreadFunctions::lockMutex(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	callInfo->getArg(0)->cast<HandleVariant>()->castHandle<MutexHandle>()->lock();
	return nullptr;
}
VariantReference<> ThreadFunctions::unlockMutex(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	callInfo->getArg(0)->cast<HandleVariant>()->castHandle<MutexHandle>()->unlock();
	return nullptr;
}

void ThreadFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<ThreadFunctions> instance(new ThreadFunctions());

	machine->addBuiltInFunction(L"createmutex",std::bind(&createMutex,instance,_1));
	machine->addBuiltInFunction(L"lockmutex",std::bind(&lockMutex,instance,_1));
	machine->addBuiltInFunction(L"unlockmutex",std::bind(&unlockMutex,instance,_1));
}

ThreadFunctions::~ThreadFunctions(void)
{

}