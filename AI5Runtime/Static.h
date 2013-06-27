#pragma once
#include "types.h"
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
	StaticData(STATIC_TYPE type);
	STATIC_TYPE getType();
private:
	STATIC_TYPE type;

};

class StaticString: public StaticData
{
public:
	StaticString(shared_string str);
	StringVariant* getVariant();
private:
	StringVariant strVar;
};

class StaticName: public StaticData
{
public:
	StaticName(shared_string str);
	shared_string getName();
private:
	shared_string name;
};


class StaticMacro: public StaticData
{
public:
	StaticMacro(shared_string str);
	shared_string getName();
private:
	shared_string name;
};


class StaticInteger64: public StaticData
{
public:
	StaticInteger64(__int64 value);
	Integer64Variant* getVariant();
private:
	Integer64Variant variant;
};


class StaticInteger32: public StaticData
{
public:
	StaticInteger32(int value);
	Integer32Variant* getVariant();
private:
	Integer32Variant variant;
};


class StaticFloating: public StaticData
{
public:
	StaticFloating(double value);
	FloatingVariant* getVariant();
private:
	FloatingVariant variant;
};


