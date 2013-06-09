#pragma once
#include "variant.h"
class BooleanVariant :
	public Variant
{
public:
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	std::wostream& format(std::wostream& stream);
	static BooleanVariant True;
	static BooleanVariant False;
	static BooleanVariant* Get(bool value);
	const bool value;
	virtual shared_string toString();
private:
	BooleanVariant(bool value);
	~BooleanVariant(void);
};

