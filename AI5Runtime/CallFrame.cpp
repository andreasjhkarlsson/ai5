#include "CallFrame.h"


CallFrame::CallFrame(int returnAddress): returnAddress(returnAddress), locals(1024)
{
}


CallFrame::~CallFrame(void)
{
}


