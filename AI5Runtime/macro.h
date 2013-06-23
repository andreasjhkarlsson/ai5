#pragma once
#include "Variant.h"
#include <functional>
class StackMachine;

typedef Variant*(*MACRO_FUNCTION_POINTER)();
typedef std::function<Variant*()> MACRO_FUNCTION;