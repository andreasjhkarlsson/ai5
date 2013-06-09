#pragma once
#include <wchar.h>
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

class Int32Comparator: public Comparator
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

class Int64Comparator: public Comparator
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

class FloatingComparator: public Comparator
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

class StringComparator: public Comparator
{
public:
	virtual bool greater(Variant* var1,Variant* var2)
	{
		return _wcsicmp(var1->toString()->c_str(),var2->toString()->c_str()) > 0;
	}
	virtual bool lesser(Variant* var1,Variant* var2)
	{
		return _wcsicmp(var1->toString()->c_str(),var2->toString()->c_str()) < 0;
	}
	virtual bool greaterEqual(Variant* var1,Variant* var2)
	{
		return _wcsicmp(var1->toString()->c_str(),var2->toString()->c_str()) >= 0;
	}
	virtual bool lesserEqual(Variant* var1,Variant* var2)
	{
		return _wcsicmp(var1->toString()->c_str(),var2->toString()->c_str()) <= 0;
	}
	virtual bool equal(Variant* var1,Variant* var2)
	{
		shared_string str1 = var1->toString();
		shared_string str2 = var2->toString();

		if(str1->length() != str2->length()) return false;

		return _wcsicmp(str1->c_str(),str2->c_str()) == 0;
	} 

	virtual bool strongEqual(Variant* var1,Variant* var2)
	{
		shared_string str1 = var1->toString();
		shared_string str2 = var2->toString();

		if(str1->length() != str2->length()) return false;

		return wcscmp(str1->c_str(),str2->c_str()) == 0;
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
			{Variant::INTEGER32,	Variant::INTEGER32,		&int32Comparator},
			{Variant::INTEGER64,	Variant::INTEGER64,		&int64Comparator},
			{Variant::INTEGER32,	Variant::INTEGER64,		&int64Comparator},
			{Variant::INTEGER64,	Variant::INTEGER32,		&int64Comparator},
			{Variant::FLOATING,		Variant::FLOATING,		&floatingComparator},
			{Variant::FLOATING,		Variant::INTEGER32,		&floatingComparator},
			{Variant::FLOATING,		Variant::INTEGER64,		&floatingComparator},
			{Variant::FLOATING,		Variant::STRING,		&floatingComparator},
			{Variant::INTEGER32,	Variant::FLOATING,		&floatingComparator},
			{Variant::INTEGER64,	Variant::FLOATING,		&floatingComparator},
			{Variant::STRING,		Variant::FLOATING,		&floatingComparator},
			{Variant::STRING,		Variant::INTEGER32,		&floatingComparator},
			{Variant::STRING,		Variant::INTEGER64,		&floatingComparator},
			{Variant::INTEGER32,	Variant::STRING,		&floatingComparator},
			{Variant::INTEGER64,	Variant::STRING,		&floatingComparator},
			{Variant::STRING,		Variant::STRING,		&stringComparator},
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
	Int32Comparator int32Comparator;
	Int64Comparator int64Comparator;
	FloatingComparator floatingComparator;
	StringComparator stringComparator;
	ComparisonPair* table;

};


enum COMPARISON_TYPE
{
	GREATER,
	GREATER_EQUAL,
	LESSER,
	LESSER_EQUAL,
	EQUAL,
	STRONG_STRING_EQUAL
};

inline void comparisonInstruction(StackMachine* machine,COMPARISON_TYPE type)
{
	static ComparisonTable table;
	static StringComparator stringComparator;
	Variant* operand2 = machine->getDataStack()->pop();
	Variant* operand1 = machine->getDataStack()->pop();
	bool result;
	Comparator* comp = table.lookup(operand1->getType(),operand2->getType());

	if(comp == nullptr)
	{
		throw new RuntimeError(L"No suitable comparator found for values");
	}

	

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
	case STRONG_STRING_EQUAL:
		result = stringComparator.strongEqual(operand1,operand2);
		break;
	}

	operand1->release();
	operand2->release();

	Variant* returnValue = BooleanVariant::Get(result);

	returnValue->addRef();

	machine->getDataStack()->push(returnValue);

	machine->advanceCounter();


}