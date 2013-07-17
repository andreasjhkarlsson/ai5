#pragma once
#include "functionvariant.h"
#include "Scope.h"
class UserFunctionVariant :
	public FunctionVariant
{
public:
	static const VARIANT_TYPE TYPE = USER_FUNCTION;
	UserFunctionVariant(int address);
	~UserFunctionVariant(void);
	std::wostream& format(std::wostream& stream);
	int getAddress();
	VariantReference<Scope>& getEnclosingScope();
	void setEnclosingScope(const VariantReference<Scope>& scope);
	virtual void cleanup();
	virtual bool equal(Variant*);
private:
	int address;
	VariantReference<Scope> enclosingScope;
};
			
