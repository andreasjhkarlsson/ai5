#include "NameStorage.h"


NameStorage::NameStorage(int tableSize): nameTable(tableSize)
{
	for(int i=0;i<tableSize;i++)
	{
		nameTable[i] = nullptr;
	}
}

NameStorage::~NameStorage(void)
{
}

