#pragma once
#include "Variant.h"
#include <functional>
class StackMachine;

typedef Variant*(*MACRO_FUNCTION_POINTER)(StackMachine*);
typedef std::function<Variant*(StackMachine*)> MACRO_FUNCTION;