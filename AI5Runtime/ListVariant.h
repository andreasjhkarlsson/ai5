#pragma once
#include "variant.h"
#include "types.h"
class ListVariant :
	public Variant
{
public:
	ListVariant(void);
	~ListVariant(void);
	std::wostream& format(std::wostream& stream);
	virtual double toFloating();
	virtual __int64 toInteger64();
	virtual int toInteger32();
	virtual bool toBoolean();
	virtual bool equal(Variant*);
	virtual shared_string toString();
	void addElement(Variant* var);
	Variant* getElement(size_t index);
	void setElement(size_t index,Variant* var);
	void deleteAt(size_t index);
	size_t size();
	void cleanup();
private:
	shared_var_list list;
};

