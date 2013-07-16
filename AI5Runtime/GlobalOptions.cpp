#include "GlobalOptions.h"


bool GlobalOptions::isVerbose()
{
	return verbose;
}

void GlobalOptions::setVerbose()
{
	verbose = true;
}

bool GlobalOptions::verbose;