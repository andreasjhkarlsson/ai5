#include "DataStack.h"
#include "VariantReference.h"


void DataStack::pushNull()
{
	push(VariantReference<>::NullReference());
}