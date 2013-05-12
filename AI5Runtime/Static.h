#pragma once

#include <vector>
#include <memory>

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


class StaticName: public StaticData
{
public:
	StaticName(const std::string& name): StaticData(NAME), name(name)
	{

	}
	__forceinline std::string& getName()
	{
		return name;
	}
private:
	std::string name;
};



