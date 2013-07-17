#pragma once

#include "FastStack.h"
#include "VariantReference.h"

class DataStack: public FastStack<VariantReference<>>
{
public:
	DataStack(int size): FastStack(size)
	{
	}
	void pushNull();
	size_t position()
	{
		return size();
	}
};