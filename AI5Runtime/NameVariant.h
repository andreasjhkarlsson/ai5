#pragma once
#include "variant.h"
#include "types.h"
class NameVariant :
	public Variant
{
public:
	NameVariant(Variant* value);
	~NameVariant(void);
	std::wostream& format(std::wostream& stream);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
	virtual bool equal(Variant*);
	virtual void cleanup();
	void markAsConst();
	bool isConstName();
	Variant* getValue();
	void setValue(Variant* variant);
	static NameVariant* createFromFactory(VariantFactory* factory);

	// If value has this instance as last name, clear it!
	void clearLastName();
private:
	Variant* value;
	bool isConst;
};

