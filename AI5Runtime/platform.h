#pragma once
#include <thread>

#ifdef _MSC_VER

#include <Windows.h>
typedef CRITICAL_SECTION LightWeightMutex;

#endif

void KillNativeThread(std::thread&);
void SuspendNativeThread(std::thread&);
void ResumeNativeThread(std::thread&);
void LockLightWeightMutex(LightWeightMutex*);
void UnlockLightWeightMutex(LightWeightMutex*);
void InitializeLightWeightMutex(LightWeightMutex*);