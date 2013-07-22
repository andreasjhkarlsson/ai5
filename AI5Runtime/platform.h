#pragma once
#include <thread>

void KillNativeThread(std::thread&);
void SuspendNativeThread(std::thread&);
void ResumeNativeThread(std::thread&);