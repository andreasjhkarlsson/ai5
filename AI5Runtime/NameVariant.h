#pragma once
#include "variant.h"
#include "types.h"
class NameVariant :
	public Variant
{
public:
	NameVariant(Variant* value);
	~NameVariant(void);
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
	virtual void cleanup();
	void markAsConst();
	Variant* getValue();
	void setValue(Variant* variant);
	static NameVariant* createFromFactory(VariantFactory* factory);
private:
	Variant* value;
	bool isConst;
};

