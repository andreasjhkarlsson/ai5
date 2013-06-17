#pragma once
#include "variant.h"
class ContainerVariant :
	public Variant
{
public:
	ContainerVariant(VARIANT_TYPE type);
	~ContainerVariant(void);
	virtual int getChildContainersCount()=0;
	virtual ContainerVariant* getChildContainer(int index)=0;
};

