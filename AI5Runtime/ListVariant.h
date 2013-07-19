#pragma once
#include "variant.h"
#include "types.h"
#include "IteratorVariant.h"
#include "VariantReference.h"

class ListVariant :
	public Variant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = LIST;
	static ListVariant* Create();
	~ListVariant(void);
	std::wostream& format(std::wostream& stream) const;
	virtual double toFloating() const;
	virtual __int64 toInteger64() const;
	virtual int toInteger32() const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	virtual shared_string toString() const;
	void addElement(const VariantReference<>& var);
	const VariantReference<>& getElement(size_t index) const;
	void setElement(size_t index,const VariantReference<>& var);
	void deleteAt(size_t index);
	size_t size() const;
	virtual VariantReference<IteratorVariant> iterate();
private:
	ListVariant(void);
	shared_var_list list;


};

