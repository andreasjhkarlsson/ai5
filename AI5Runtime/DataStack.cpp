#include "DataStack.h"
#include "NullVariant.h"


void DataStack::pushNull()
{
	NullVariant::Instance.addRef();
	push(&NullVariant::Instance);
}