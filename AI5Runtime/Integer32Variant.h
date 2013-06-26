#pragma once
#include "variant.h"
class Integer32Variant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = INTEGER32;
	Integer32Variant(int value);
	~Integer32Variant(void);
	std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual shared_string toString() const;
	virtual bool equal(Variant*);
	virtual size_t hash() const;
	friend class VariantFactory;
	static Integer32Variant* createFromFactory(VariantFactory* factory,int value);
	int getValue()
	{
		return value;
	}
	void setValue(int value)
	{
		this->value = value;
	}
private:
	int value;
};

