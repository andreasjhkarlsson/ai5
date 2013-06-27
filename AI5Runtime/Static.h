#pragma once

#include <vector>
#include <memory>
#include "StringVariant.h"
#include "FloatingVariant.h"
#include "Integer64Variant.h"
#include "Integer32Variant.h"

typedef unsigned char STATIC_TYPE;


class StaticData
{
public:
	typedef std::shared_ptr<StaticData> PTR;
	const static STATIC_TYPE STRING = 0;
	const static STATIC_TYPE BINARY = 1;
	const static STATIC_TYPE NAME = 2;
	const static STATIC_TYPE FLOATING = 3;
	const static STATIC_TYPE INTEGER64 = 4;
	const static STATIC_TYPE INTEGER32 = 5;
	const static STATIC_TYPE MACRO = 6;
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
	StaticString(shared_string str): StaticData(STRING), strVar(str)
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
	StaticName(shared_string str): StaticData(NAME),name(str)
	{

	}

	__forceinline shared_string getName()
	{
		return name;
	}
private:
	shared_string name;
};


class StaticMacro: public StaticData
{
public:
	StaticMacro(shared_string str): StaticData(MACRO),name(str)
	{

	}

	__forceinline shared_string getName()
	{
		return name;
	}
private:
	shared_string name;
};


class StaticInteger64: public StaticData
{
public:
	StaticInteger64(__int64 value): StaticData(INTEGER64),variant(value)
	{

	}

	__forceinline Integer64Variant* getVariant()
	{
		return &variant;
	}
private:
	Integer64Variant variant;
};


class StaticInteger32: public StaticData
{
public:
	StaticInteger32(int value): StaticData(INTEGER32),variant(value)
	{

	}

	__forceinline Integer32Variant* getVariant()
	{
		return &variant;
	}
private:
	Integer32Variant variant;
};


class StaticFloating: public StaticData
{
public:
	StaticFloating(double value): StaticData(FLOATING),variant(value)
	{

	}

	__forceinline FloatingVariant* getVariant()
	{
		return &variant;
	}
private:
	FloatingVariant variant;
};


