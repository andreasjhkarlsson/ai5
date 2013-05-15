#include "Variant.h"
#include "IntegerVariant.h"
#include "StackMachine.h"
#include "BooleanVariant.h"





__forceinline void addition(StackMachine* machine)
{
	// Pop the arguments in reverse.
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();

	// TODO: Make sure this works for more than 2 integers.
	if(v1->getType() == Variant::INTEGER && v2->getType() == Variant::INTEGER)
	{
		// Cast to correct variant instead of using toInteger method. This saves a virtual call.
		__int64 res = ((IntegerVariant*)v1)->getValue()+((IntegerVariant*)v2)->getValue();

		// Push result on stack.
		machine->getDataStack()->push(machine->getVariantFactory()->create<IntegerVariant,__int64>(Variant::INTEGER,res));
	}

	// Release arguments.
	v1->release();
	v2->release();
	
	machine->advanceCounter();
}


__forceinline void multiplication(StackMachine* machine)
{
	// Pop the arguments in reverse.
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();

	// TODO: Make sure this works for more than 2 integers.
	if(v1->getType() == Variant::INTEGER && v2->getType() == Variant::INTEGER)
	{
		// Cast to correct variant instead of using toInteger method. This saves a virtual call.
		__int64 res = ((IntegerVariant*)v1)->getValue()*((IntegerVariant*)v2)->getValue();

		// Push result on stack.
		machine->getDataStack()->push(machine->getVariantFactory()->create<IntegerVariant,__int64>(Variant::INTEGER,res));
	}

	// Release arguments.
	v1->release();
	v2->release();
	
	machine->advanceCounter();
}


__forceinline void subtraction(StackMachine* machine)
{
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();


	if(v1->getType() == Variant::INTEGER && v2->getType() == Variant::INTEGER)
	{
		__int64 res = ((IntegerVariant*)v1)->getValue()-((IntegerVariant*)v2)->getValue();

		machine->getDataStack()->push(machine->getVariantFactory()->create<IntegerVariant,__int64>(Variant::INTEGER,res));
	}

	v1->release();
	v2->release();
	
	machine->advanceCounter();
}

__forceinline void division(StackMachine* machine)
{
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();


	if(v1->getType() == Variant::INTEGER && v2->getType() == Variant::INTEGER)
	{
		__int64 res = ((IntegerVariant*)v1)->getValue()/((IntegerVariant*)v2)->getValue();

		machine->getDataStack()->push(machine->getVariantFactory()->create<IntegerVariant,__int64>(Variant::INTEGER,res));
	}

	v1->release();
	v2->release();
	
	machine->advanceCounter();
}

__forceinline void greater(StackMachine* machine)
{
	Variant *v2 = machine->getDataStack()->pop();
	Variant *v1 = machine->getDataStack()->pop();

	if(v1->getType() == Variant::INTEGER && v2->getType() == Variant::INTEGER)
	{
		BooleanVariant *result;
		if (((IntegerVariant*)v1)->getValue()>((IntegerVariant*)v2)->getValue())
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

