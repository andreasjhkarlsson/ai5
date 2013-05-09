#pragma once
#include "variant.h"

class ReferenceVariant: public Variant
{
public:
	ReferenceVariant(Variant*);
	virtual ~ReferenceVariant();
	void assign(Variant*);
	void unlink();
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	__forceinline Variant* deref();
private:
	Variant* var;
};

Variant* ReferenceVariant::deref()
{
	return var;
}