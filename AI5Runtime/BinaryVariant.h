#pragma once
#include "variant.h"
#include <vector>

typedef std::shared_ptr<std::vector<char>> shared_binary;

class BinaryVariant :
	public Variant
{
public:
	BinaryVariant(shared_binary binary);
	~BinaryVariant(void);
	shared_binary getValue();
	virtual std::wostream& format(std::wostream& stream) const;
	virtual bool toBoolean() const;
	virtual bool equal(Variant*);
	virtual shared_string toString() const;
	virtual size_t hash() const;
private:
	shared_binary binary;
};

