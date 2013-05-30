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


__forceinline void mathOperation(StackMachine* machine,MATH_OPERATION type)
{
	// Pop the arguments in reverse.
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();

	Variant* returnValue = nullptr;

	// If the arguments is two integers and the operation is not a division or a pow
	// the result should be an int as well!!
	if(v1->getType() == Variant::INTEGER64 && v2->getType() == Variant::INTEGER64 && type != MATH_OPERATION::DIVISION && type != MATH_OPERATION::POW)
	{
		// Cast to correct variant instead of using toInteger method. This saves a virtual call.
		__int64 res = 0;
		__int64 i1 = ((Integer64Variant*)v1)->getValue();
		__int64 i2 = ((Integer64Variant*)v2)->getValue();

		switch(type)
		{
		case ADDITION:
			res = i1+i2;
			break;
		case SUBTRACTION:
			res = i1-i2;
			break;
		case MULTIPLICATION:
			res = i1*i2;
			break;

		}

		returnValue = Integer64Variant::createFromFactory(machine->getVariantFactory(),res);
	}
	// Else the result is a double.
	else
	{
		double res = 0.0;
		double d1;
		double d2;
		if(v1->getType() == Variant::FLOATING && v2->getType() == Variant::FLOATING)
		{
			d1 = ((FloatingVariant*)v1)->getValue();
			d2 = ((FloatingVariant*)v2)->getValue();
		}
		else
		{
			d1 = v1->toFloating();
			d2 = v2->toFloating();
		}

		switch(type)
		{
		case ADDITION:
			res = d1+d2;
			break;
		case SUBTRACTION:
			res = d1-d2;
			break;
		case MULTIPLICATION:
			res = d1*d2;
			break;
		case DIVISION:
			res = d1/d2;
			break;
		case POW:
			res = pow(d2,d2);
			break;
		}

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

	v1->release();
	v2->release();

	machine->advanceCounter();
}
