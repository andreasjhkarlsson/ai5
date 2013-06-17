#pragma once
#include "containervariant.h"
#include "types.h"
class NameVariant :
	public ContainerVariant
{
public:
	NameVariant(Variant* value,VARIANT_TYPE type = NAME);
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

