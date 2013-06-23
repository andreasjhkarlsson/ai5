#pragma once
#include "variant.h"
class Integer32Variant :
	public Variant
{
public:
	Integer32Variant(int value);
	~Integer32Variant(void);
	std::wostream& format(std::wostream& stream);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
	virtual bool equal(Variant*);
	virtual size_t hash();
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

