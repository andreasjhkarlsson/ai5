#include "Variant.h"
#include "StackMachineThread.h"
#include <cmath>

enum MATH_OPERATION
{
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	POW
};

template<typename T>
__forceinline T mathOperation(MATH_OPERATION type,T v1,T v2)
{
	T res = 0;
	switch(type)
	{
	case ADDITION:
		res = v1+v2;
		break;
	case SUBTRACTION:
		res = v1-v2;
		break;
	case MULTIPLICATION:
		res = v1*v2;
		break;
	case DIVISION:
		res = v1/v2;
		break;
	case POW:
		res = (T)pow((double)v1,(double)v2);
		break;
	}
	return res;
}

__forceinline void mathOperationInstruction(StackMachineThread* machine,MATH_OPERATION type)
{
	// Pop the arguments in reverse.
	VariantReference<> r2 = machine->getDataStack()->pop();
	VariantReference<> r1 = machine->getDataStack()->pop();

	VariantReference<> returnValue = nullptr;

	// If the arguments is two integers and the operation is not a division or a pow
	// the result should be an int as well!!
	if(r1.isIntegerType() && r2.isIntegerType() &&  type != MATH_OPERATION::DIVISION && type != MATH_OPERATION::POW)
	{
		bool useInt64 = r1.isInteger64Type() || r2.isInteger64Type();

		if(useInt64)
		{
			returnValue = VariantReference<>(mathOperation<__int64>(type,r1.toInteger64(),r2.toInteger64()));
		}
		else
		{
			returnValue = VariantReference<>(mathOperation<int>(type,r1.toInteger32(),r2.toInteger32()));
		}
	}
	// Else the result is a double.
	else
	{
		double res = 0.0;
		double d1 = r1.toFloating();
		double d2 = r2.toFloating();

		res = mathOperation<double>(type,d1,d2);

		returnValue = VariantReference<>(res);
	}

	// Push result on stack.
	machine->getDataStack()->push(returnValue);

	
	machine->advanceCounter();	
}

_forceinline void negation(StackMachineThread* machine)
{
	VariantReference<> arg = machine->getDataStack()->pop();

	if(arg.isInteger32Type())
	{
		machine->getDataStack()->push(VariantReference<>(-arg.toInteger32()));
	}
	else if (arg.isInteger64Type())
	{
		machine->getDataStack()->push(VariantReference<>(-arg.toInteger64()));
	}
	else
	{
		machine->getDataStack()->push(VariantReference<>(-arg.toFloating()));
	}

	machine->advanceCounter();
}

__forceinline void booleanNot(StackMachineThread* machine)
{
	VariantReference<> argument = machine->getDataStack()->pop();
	VariantReference<> result = VariantReference<>(!argument.toBoolean());

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

__forceinline void booleanAnd(StackMachineThread* machine)
{
	VariantReference<> argument2 = machine->getDataStack()->pop();
	VariantReference<> argument1 = machine->getDataStack()->pop();
	VariantReference<> result = VariantReference<>(argument1.toBoolean() && argument2.toBoolean());

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

__forceinline void booleanOr(StackMachineThread* machine)
{
	VariantReference<> argument2 = machine->getDataStack()->pop();
	VariantReference<> argument1 = machine->getDataStack()->pop();
	VariantReference<> result = VariantReference<>(argument1.toBoolean() || argument2.toBoolean());

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

