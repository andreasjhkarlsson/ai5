#include "platform.h"
#ifdef _MSC_VER

#include <Windows.h>
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

#endif