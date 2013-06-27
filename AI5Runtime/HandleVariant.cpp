#include "HandleVariant.h"
#include "RuntimeError.h"

HandleVariant::HandleVariant(HANDLE_TYPE handleType): Variant(TYPE), handleType(handleType)
{
}


HandleVariant::~HandleVariant(void)
{
}