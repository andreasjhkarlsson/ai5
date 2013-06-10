#pragma once
#include "variant.h"
class DefaultVariant :
	public Variant
{
public:
	DefaultVariant(void);
	~DefaultVariant(void);
	virtual std::wostream& format(std::wostream& stream);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
	virtual bool equal(Variant*);
	static DefaultVariant Instance;
};

