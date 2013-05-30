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
							static_cast<Integer64Variant*>(v1)->getValue(),static_cast<Integer64Variant*>(v2)->getValue()));
		}
		else
		{
			returnValue = Integer32Variant::createFromFactory(machine->getVariantFactory(),mathOperation<int>(type,
							static_cast<Integer32Variant*>(v1)->getValue(),static_cast<Integer32Variant*>(v2)->getValue()));
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

__forceinline void greater(StackMachine* machine)
{
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();

	if(v1->getType() == Variant::INTEGER64 && v2->getType() == Variant::INTEGER64)
	{
		BooleanVariant *result;
		if (((Integer64Variant*)v1)->getValue()>((Integer64Variant*)v2)->getValue())
			result = &BooleanVariant::True;
		else
			result = &BooleanVariant::False;
		result->addRef();
		machine->getDataStack()->push(result);
	}

	if(v1->getType() == Variant::INTEGER32 && v2->getType() == Variant::INTEGER32)
	{
		BooleanVariant *result;
		if (((Integer32Variant*)v1)->getValue()>((Integer32Variant*)v2)->getValue())
			result = &BooleanVariant::True;
		else
			result = &BooleanVariant::False;
		result->addRef();
		machine->getDataStack()->push(result);
	}

	v1->release();
	v2->release();

	machine->advanceCounter();
}
