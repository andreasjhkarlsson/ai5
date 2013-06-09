#pragma once
#include "Block.h"
#include "FastStack.h"


class BlockStack: public FastStack<Block*>
{
public:
	BlockStack(int limit): FastStack(limit)
	{

	}

};