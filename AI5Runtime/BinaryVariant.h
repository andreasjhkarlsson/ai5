#pragma once
#include "variant.h"

class BinaryVariant :
	public Variant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = BINARY;
	static BinaryVariant* Create(shared_binary);
	~BinaryVariant(void);
	shared_binary getValue();
	virtual std::wostream& format(std::wostream& stream) const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	virtual shared_string toString() const;
	virtual size_t hash() const;
private:
	shared_binary binary;
	BinaryVariant(shared_binary binary);
};

