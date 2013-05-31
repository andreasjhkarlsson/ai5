#include "Variant.h"
#include "Integer64Variant.h"
#include "StackMachine.h"
#include "BooleanVariant.h"
#include "FloatingVariant.h"
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

__forceinline void mathOperationInstruction(StackMachine* machine,MATH_OPERATION type)
{
	// Pop the arguments in reverse.
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();

	Variant* returnValue = nullptr;

	// If the arguments is two integers and the operation is not a division or a pow
	// the result should be an int as well!!
	if(v1->isIntegerType() && v2->isIntegerType() &&  type != MATH_OPERATION::DIVISION && type != MATH_OPERATION::POW)
	{
		bool useInt64 = v1->isInteger64Type() || v2->isInteger64Type();

		if(useInt64)
		{
			returnValue = Integer64Variant::createFromFactory(machine->getVariantFactory(),mathOperation<__int64>(type,
				v1->toInteger64(),v2->toInteger64()));
		}
		else
		{
			returnValue = Integer32Variant::createFromFactory(machine->getVariantFactory(),mathOperation<int>(type,
				v1->toInteger32(),v2->toInteger32()));
		}
	}
	// Else the result is a double.
	else
	{
		double res = 0.0;
		double d1 = v1->toFloating();
		double d2 = v2->toFloating();

		res = mathOperation<double>(type,d1,d2);

		returnValue = FloatingVariant::createFromFactory(machine->getVariantFactory(),res);
	}

	// Push result on stack.
	machine->getDataStack()->push(returnValue);

	// Release arguments.
	v1->release();
	v2->release();
	
	machine->advanceCounter();	
}

__forceinline void booleanNot(StackMachine* machine)
{
	Variant *argument = machine->getDataStack()->pop();
	Variant* result = nullptr;

	if(!argument->toBoolean())
		result = &BooleanVariant::True;
	else 
		result = &BooleanVariant::False;
	
	argument->release();

	result->addRef();

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

__forceinline void booleanAnd(StackMachine* machine)
{
	Variant *argument2 = machine->getDataStack()->pop();
	Variant *argument1 = machine->getDataStack()->pop();
	Variant* result = nullptr;

	if(argument1->toBoolean() && argument2->toBoolean())
		result = &BooleanVariant::True;
	else 
		result = &BooleanVariant::False;
	
	argument1->release();
	argument2->release();

	result->addRef();

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

__forceinline void booleanOr(StackMachine* machine)
{
	Variant *argument2 = machine->getDataStack()->pop();
	Variant *argument1 = machine->getDataStack()->pop();
	Variant* result = nullptr;

	if(argument1->toBoolean() || argument2->toBoolean())
		result = &BooleanVariant::True;
	else 
		result = &BooleanVariant::False;
	
	argument1->release();
	argument2->release();

	result->addRef();

	machine->getDataStack()->push(result);

	machine->advanceCounter();
}

