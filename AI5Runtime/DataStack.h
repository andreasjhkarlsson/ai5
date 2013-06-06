#pragma once

#include "FastStack.h"
#include "Variant.h"

class DataStack: public FastStack<Variant*>
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