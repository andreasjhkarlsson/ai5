#include "DataStack.h"
#include "NullVariant.h"


void DataStack::pushNull()
{
	push(&NullVariant::Instance);
}