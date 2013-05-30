#pragma once
#include "Variant.h"
#include "BooleanVariant.h"
#include "StackMachine.h"

class Comparator
{
public:
	virtual bool greater(Variant* var1,Variant* var2)=0;
	virtual bool lesser(Variant* var1,Variant* var2)=0;
	virtual bool greaterEqual(Variant* var1,Variant* var2)=0;
	virtual bool lesserEqual(Variant* var1,Variant* var2)=0;
	virtual bool equal(Variant* var1,Variant* var2)=0;
};

class TInt32Comparator: public Comparator
{
public:
	virtual bool greater(Variant* var1,Variant* var2)
	{
		return var1->toInteger32() > var2->toInteger32();
	}
	virtual bool lesser(Variant* var1,Variant* var2)
	{
		return var1->toInteger32() < var2->toInteger32();
	}
	virtual bool greaterEqual(Variant* var1,Variant* var2)
	{
		return var1->toInteger32() >= var2->toInteger32();
	}
	virtual bool lesserEqual(Variant* var1,Variant* var2)
	{
		return var1->toInteger32() <= var2->toInteger32();
	}
	virtual bool equal(Variant* var1,Variant* var2)
	{
		return var1->toInteger32() == var2->toInteger32();
	}
};

class TInt64Comparator: public Comparator
{
public:
	virtual bool greater(Variant* var1,Variant* var2)
	{
		return var1->toInteger64() > var2->toInteger64();
	}
	virtual bool lesser(Variant* var1,Variant* var2)
	{
		return var1->toInteger64() < var2->toInteger64();
	}
	virtual bool greaterEqual(Variant* var1,Variant* var2)
	{
		return var1->toInteger64() >= var2->toInteger64();
	}
	virtual bool lesserEqual(Variant* var1,Variant* var2)
	{
		return var1->toInteger64() <= var2->toInteger64();
	}
	virtual bool equal(Variant* var1,Variant* var2)
	{
		return var1->toInteger64() == var2->toInteger64();
	}
};



class TFloatingComparator: public Comparator
{
public:
	virtual bool greater(Variant* var1,Variant* var2)
	{
		return var1->toFloating() > var2->toFloating();
	}
	virtual bool lesser(Variant* var1,Variant* var2)
	{
		return var1->toFloating() < var2->toFloating();
	}
	virtual bool greaterEqual(Variant* var1,Variant* var2)
	{
		return var1->toFloating() >= var2->toFloating();
	}
	virtual bool lesserEqual(Variant* var1,Variant* var2)
	{
		return var1->toFloating() <= var2->toFloating();
	}
	virtual bool equal(Variant* var1,Variant* var2)
	{
		return var1->toFloating() == var2->toFloating();
	} 
};

struct ComparisonPair
{
	VARIANT_TYPE type1, type2;
	Comparator* comparator;
};

class ComparisonTable
{
public:
	ComparisonTable()
	{

		// Table describing the rules for comparisons.
		ComparisonPair RAW_TABLE[] = {
			{Variant::INTEGER32,	Variant::INTEGER32,		&Int32Comparator},
			{Variant::INTEGER64,	Variant::INTEGER64,		&Int64Comparator},
			{Variant::INTEGER32,	Variant::INTEGER64,		&Int64Comparator},
			{Variant::INTEGER64,	Variant::INTEGER32,		&Int64Comparator},
			{Variant::FLOATING,		Variant::FLOATING,		&FloatingComparator},
			{Variant::FLOATING,		Variant::INTEGER32,		&FloatingComparator},
			{Variant::FLOATING,		Variant::INTEGER64,		&FloatingComparator},
			{Variant::FLOATING,		Variant::STRING,		&FloatingComparator},
			{Variant::INTEGER32,	Variant::FLOATING,		&FloatingComparator},
			{Variant::INTEGER64,	Variant::FLOATING,		&FloatingComparator},
			{Variant::STRING,		Variant::FLOATING,		&FloatingComparator},
			{Variant::STRING,		Variant::INTEGER32,		&FloatingComparator},
			{Variant::STRING,		Variant::INTEGER64,		&FloatingComparator},
		};


		int num_variants = Variant::NUMBER_OF_VARIANT_TYPES;
		table = new ComparisonPair[num_variants*num_variants];
		memset(table,0,num_variants*num_variants);
		for(int i=0;i<(sizeof(RAW_TABLE)/sizeof(ComparisonPair));i++)
		{
			table[RAW_TABLE[i].type1*num_variants+RAW_TABLE[i].type2] = RAW_TABLE[i];
		}
	}

	Comparator* lookup(VARIANT_TYPE type1,VARIANT_TYPE type2)
	{
		return table[type1*Variant::NUMBER_OF_VARIANT_TYPES+type2].comparator;
	}

private:
	TInt32Comparator Int32Comparator;
	TInt64Comparator Int64Comparator;
	TFloatingComparator FloatingComparator;
	ComparisonPair* table;

};


enum COMPARISON_TYPE
{
	GREATER,
	GREATER_EQUAL,
	LESSER,
	LESSER_EQUAL,
	EQUAL
};

inline void comparisonInstruction(StackMachine* machine,COMPARISON_TYPE type)
{
	static ComparisonTable table;
	Variant* operand2 = machine->getDataStack()->pop();
	Variant* operand1 = machine->getDataStack()->pop();
	bool result;
	Comparator* comp = table.lookup(operand1->getType(),operand2->getType());

	switch(type)
	{
	case GREATER:
		result = comp->greater(operand1,operand2);
		break;
	case GREATER_EQUAL:
		result = comp->greaterEqual(operand1,operand2);
		break;
	case LESSER:
		result = comp->lesser(operand1,operand2);
		break;
	case LESSER_EQUAL:
		result = comp->lesserEqual(operand1,operand2);
		break;
	case EQUAL:
		result = comp->equal(operand1,operand2);
		break;
	}

	operand1->release();
	operand2->release();

	Variant* returnValue = BooleanVariant::Get(result);

	returnValue->addRef();

	machine->getDataStack()->push(returnValue);

	machine->advanceCounter();


}