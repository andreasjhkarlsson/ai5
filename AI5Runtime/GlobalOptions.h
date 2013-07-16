#pragma once
class GlobalOptions
{
public:
	static bool isVerbose();
	static void setVerbose();
private:
	static bool verbose;
};

