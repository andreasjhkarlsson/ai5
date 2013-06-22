#include "Functions.h"
#include "..\AI5Runtime\RuntimeError.h"

Functions::Functions(void)
{
}


Functions::~Functions(void)
{
}


void Functions::validateArgCount(int got,int required,int accepted)
{
	if(got < required || got > accepted)
		throw InvalidArgumentCountError(got,required,accepted);

}