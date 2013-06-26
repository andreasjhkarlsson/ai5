#pragma once
#include "variant.h"
#include "types.h"
#include "IteratorVariant.h"

class ListVariant :
	public Variant
{
public:
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
	Variant* getElement(size_t index) const;
	void setElement(size_t index,Variant* var);
	void deleteAt(size_t index);
	size_t size() const;
	void cleanup();
	virtual IteratorVariant* iterate();
private:
	shared_var_list list;

	class ForwardIterator: public IteratorVariant
	{
	public:
		ForwardIterator(ListVariant* list);
		virtual void cleanup();
		virtual bool hasMore();
		virtual Variant* next();
	private:
		int pos;
		ListVariant* list;
	};
};

