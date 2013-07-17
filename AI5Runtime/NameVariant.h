#pragma once
#include "types.h"
#include "VariantReference.h"
class NameVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = NAME;
	NameVariant(const VariantReference<>& value,VARIANT_TYPE type = TYPE);
	~NameVariant(void);
	std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
	virtual void cleanup();
	void markAsConst();
	bool isConstName();
	virtual const VariantReference<>& getValue();
	virtual void setValue(const VariantReference<>& variant);
protected:
	VariantReference<> value;
private:
	bool isConst;
};

