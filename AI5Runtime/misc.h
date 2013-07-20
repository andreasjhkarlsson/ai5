#pragma once
#include "types.h"
#include "GlobalOptions.h"
#include <iostream>

class DebugOut
{
public:

	static const int COLUMN_WIDTH = 12;

	DebugOut(const std::wstring &type)
	{
		if(GlobalOptions::isVerbose())
		{
			std::wcout << type;
			for(int i=type.length();i<COLUMN_WIDTH;i++)
				std::wcout << " ";
			std::wcout << ": ";
		}

	}

	~DebugOut()
	{
		if(GlobalOptions::isVerbose())
		{
			std::wcout << std::endl;
		}
	}

	template <typename T>
	friend DebugOut& operator<<(DebugOut& out,T i)
	{
		if(GlobalOptions::isVerbose())
			std::wcout << i;
		return out;
	}

};
