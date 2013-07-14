#pragma once
#include "Variant.h"
#include <functional>
class StackMachineThread;

typedef Variant*(*MACRO_FUNCTION_POINTER)(StackMachineThread*);
typedef std::function<Variant*(StackMachineThread*)> MACRO_FUNCTION;