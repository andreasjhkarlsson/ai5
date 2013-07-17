#pragma once
#include "Variant.h"
#include <functional>
class StackMachineThread;

typedef VariantReference<>(*MACRO_FUNCTION_POINTER)(StackMachineThread*);
typedef std::function<VariantReference<>(StackMachineThread*)> MACRO_FUNCTION;