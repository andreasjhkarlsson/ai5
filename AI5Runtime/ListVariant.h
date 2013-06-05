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
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual shared_string toString();
	void addElement(Variant* var);
	Variant* getElement(int index);
	void setElement(int index,Variant* var);
	int size();
private:
	shared_var_list list;
};

