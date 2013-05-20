#pragma once

#include <vector>
#include <memory>
#include "StringVariant.h"

typedef unsigned char STATIC_TYPE;


class StaticData
{
public:
	typedef std::shared_ptr<StaticData> PTR;
	const static STATIC_TYPE STRING = 0;
	const static STATIC_TYPE BINARY = 1;
	const static STATIC_TYPE NAME = 2;
	StaticData(STATIC_TYPE type): type(type)
	{
	}
	__forceinline STATIC_TYPE getType()
	{
		return type;
	}
private:
	STATIC_TYPE type;

};

class StaticString: public StaticData
{
public:
	StaticString(const std::wstring& str): StaticData(STRING), strVar(str)
	{

	}
	StringVariant* getVariant()
	{
		return &strVar;
	}
private:
	StringVariant strVar;
};

class StaticName: public StaticData
{
public:
	StaticName(const std::wstring& str): StaticData(NAME),name(str)
	{

	}

	__forceinline std::wstring& getName()
	{
		return name;
	}
private:
	std::wstring name;
};



