#pragma once
#include <wchar.h>
#include "Variant.h"
#include "BooleanVariant.h"
#include "StackMachineThread.h"

class Comparator
{
public:
	virtual bool greater(const VariantReference<>& var1,const VariantReference<>& var2)=0;
	virtual bool lesser(const VariantReference<>& var1,const VariantReference<>& var2)=0;
	virtual bool greaterEqual(const VariantReference<>& var1,const VariantReference<>& var2)=0;
	virtual bool lesserEqual(const VariantReference<>& var1,const VariantReference<>& var2)=0;
	virtual bool equal(const VariantReference<>& var1,const VariantReference<>& var2)=0;
};

class Int32Comparator: public Comparator
{
public:
	virtual bool greater(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger32() > var2.toInteger32();
	}
	virtual bool lesser(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger32() < var2.toInteger32();
	}
	virtual bool greaterEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger32() >= var2.toInteger32();
	}
	virtual bool lesserEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger32() <= var2.toInteger32();
	}
	virtual bool equal(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger32() == var2.toInteger32();
	}
};

class Int64Comparator: public Comparator
{
public:
	virtual bool greater(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger64() > var2.toInteger64();
	}
	virtual bool lesser(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger64() < var2.toInteger64();
	}
	virtual bool greaterEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger64() >= var2.toInteger64();
	}
	virtual bool lesserEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger64() <= var2.toInteger64();
	}
	virtual bool equal(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toInteger64() == var2.toInteger64();
	}
};

class FloatingComparator: public Comparator
{
public:
	virtual bool greater(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toFloating() > var2.toFloating();
	}
	virtual bool lesser(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toFloating() < var2.toFloating();
	}
	virtual bool greaterEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toFloating() >= var2.toFloating();
	}
	virtual bool lesserEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toFloating() <= var2.toFloating();
	}
	virtual bool equal(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toFloating() == var2.toFloating();
	} 
};

class StringComparator: public Comparator
{
public:
	virtual bool greater(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toString()->compare(*var2.toString()) > 0;
		
	}
	virtual bool lesser(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toString()->compare(*var2.toString()) < 0;
	}
	virtual bool greaterEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toString()->compare(*var2.toString()) >= 0;
	}
	virtual bool lesserEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		return var1.toString()->compare(*var2.toString()) <= 0;
	}
	virtual bool equal(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		shared_string str1 = var1.toString();
		shared_string str2 = var2.toString();

		if(str1->length() != str2->length()) return false;

		return str1->caseCompare(*str2,0) == 0;
	} 

	virtual bool strongEqual(const VariantReference<>& var1,const VariantReference<>& var2)
	{
		shared_string str1 = var1.toString();
		shared_string str2 = var2.toString();

		if(str1->length() != str2->length()) return false;

		return str1->compare(*str2) == 0;
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
		static ComparisonPair RAW_TABLE[] = {
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
		table = new ComparisonPair*[num_variants*num_variants];
		memset(table,0,sizeof(ComparisonPair*)*num_variants*num_variants);
		for(int i=0;i<(sizeof(RAW_TABLE)/sizeof(ComparisonPair));i++)
		{
			table[RAW_TABLE[i].type1*num_variants+RAW_TABLE[i].type2] = &RAW_TABLE[i];
		}
	}

	Comparator* lookup(VARIANT_TYPE type1,VARIANT_TYPE type2)
	{
		ComparisonPair* pair =  table[type1*Variant::NUMBER_OF_VARIANT_TYPES+type2];
		if(pair == nullptr)
			return nullptr;
		return pair->comparator;
	}

private:
	Int32Comparator int32Comparator;
	Int64Comparator int64Comparator;
	FloatingComparator floatingComparator;
	StringComparator stringComparator;
	ComparisonPair** table;

};


enum COMPARISON_TYPE
{
	GREATER,
	GREATER_EQUAL,
	LESSER,
	LESSER_EQUAL,
	EQUAL,
	NOT_EQUAL,
	STRONG_STRING_EQUAL,
	EXACTLY_EQUAL
};

extern "C" ComparisonTable table;
extern "C" StringComparator stringComparator;

inline void comparisonInstruction(StackMachineThread* machine,COMPARISON_TYPE type)
{

	VariantReference<> operand2 = machine->getDataStack()->pop();
	VariantReference<> operand1 = machine->getDataStack()->pop();
	bool result = false;
	Comparator* comp = table.lookup(operand1.getType(),operand2.getType());

	if(comp == nullptr && type != EXACTLY_EQUAL)
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
	case NOT_EQUAL:
		result = !comp->equal(operand1,operand2);
		break;
	case STRONG_STRING_EQUAL:
		result = stringComparator.strongEqual(operand1,operand2);
		break;
	case EXACTLY_EQUAL:
		result = operand1.equal(operand2);
		break;
	}

	machine->getDataStack()->push(result);

	machine->advanceCounter();


}