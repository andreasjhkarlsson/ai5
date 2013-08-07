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

// Code taken from: http://msdn.microsoft.com/en-us/library/xcb2z8hs.aspx
//
// Usage: SetThreadName (-1, "MainThread");
//
const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void renameNativeThread(std::thread& thread,const char* name)
{
   THREADNAME_INFO info;
   info.dwType = 0x1000;
   info.szName = name;
   info.dwThreadID = GetThreadId(thread.native_handle());
   info.dwFlags = 0;

   __try
   {
      RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
   }
   __except(EXCEPTION_EXECUTE_HANDLER)
   {
   }
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