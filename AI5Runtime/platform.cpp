#include "platform.h"
#ifdef _MSC_VER

void KillNativeThread(std::thread& thread)
{
	TerminateThread(thread.native_handle(),0);
}
void SuspendNativeThread(std::thread& thread)
{
	SuspendThread(thread.native_handle());
}

void ResumeNativeThread(std::thread& thread)
{
	ResumeThread(thread.native_handle());
}

void LockLightWeightMutex(LightWeightMutex* mutex)
{
	EnterCriticalSection(mutex);
}

void UnlockLightWeightMutex(LightWeightMutex* mutex)
{
	LeaveCriticalSection(mutex);
}

void InitializeLightWeightMutex(LightWeightMutex* mutex)
{
	InitializeCriticalSection(mutex);
}

#endif