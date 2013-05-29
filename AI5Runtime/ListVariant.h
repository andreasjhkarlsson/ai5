#pragma once
#include "variant.h"
#include "types.h"
class ListVariant :
	public Variant
{
public:
	ListVariant(void);
	~ListVariant(void);
	virtual void print();
	virtual double toFloating();
	virtual __int64 toInteger();
	virtual bool toBoolean();
	virtual shared_string toString();
	void addElement(Variant* var);
	Variant* getElement(int index);
private:
	shared_var_list list;
};

