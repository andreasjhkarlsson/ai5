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
	virtual std::wostream& format(std::wostream& stream);
	virtual bool toBoolean();
	virtual bool equal(Variant*);
	virtual shared_string toString();
	virtual size_t hash();
private:
	shared_binary binary;
};

