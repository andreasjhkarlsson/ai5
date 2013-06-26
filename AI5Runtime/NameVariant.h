#pragma once
#include "containervariant.h"
#include "types.h"
class NameVariant :
	public ContainerVariant
{
public:
	static const VARIANT_TYPE TYPE = NAME;
	NameVariant(Variant* value,VARIANT_TYPE type = TYPE);
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
	virtual Variant* getValue();
	virtual void setValue(Variant* variant);
	static NameVariant* createFromFactory(VariantFactory* factory);
	virtual int getChildContainersCount();
	virtual ContainerVariant* getChildContainer(int index);
protected:
	Variant* value;
private:
	bool isConst;
};

