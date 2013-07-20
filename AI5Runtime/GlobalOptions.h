#pragma once
class GlobalOptions
{
public:
	static bool isVerbose();
	static void setVerbose();
	static bool shouldPrintInstructions();
	static void setPrintInstructions();
private:
	static bool verbose;
	static bool printInstructions;
};

