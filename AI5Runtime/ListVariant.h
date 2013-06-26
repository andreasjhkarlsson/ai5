#pragma once
#include "variant.h"
#include "types.h"
class ListVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = LIST;
	ListVariant(void);
	~ListVariant(void);
	std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	virtual shared_string toString() const;
	void addElement(Variant* var);
	Variant* getElement(size_t index);
	void setElement(size_t index,Variant* var);
	void deleteAt(size_t index);
	size_t size();
	void cleanup();
private:
	shared_var_list list;
};

