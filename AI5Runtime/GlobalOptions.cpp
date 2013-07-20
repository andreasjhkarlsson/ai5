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

bool GlobalOptions::shouldPrintInstructions()
{
	return printInstructions;
}

void GlobalOptions::setPrintInstructions()
{
	printInstructions = true;
}

bool GlobalOptions::printInstructions;