#pragma once
#include "variant.h"
#include <string>
class StringVariant :
	public Variant
{
public:
	StringVariant(const std::wstring& str);
	~StringVariant(void);
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	const std::wstring& getString();
private:
	std::wstring str;
};

