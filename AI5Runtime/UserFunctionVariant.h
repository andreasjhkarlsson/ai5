#pragma once
#include "functionvariant.h"
#include "Scope.h"
class UserFunctionVariant :
	public FunctionVariant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = USER_FUNCTION;
	~UserFunctionVariant(void);
	std::wostream& format(std::wostream& stream);
	int getAddress();
	VariantReference<Scope>& getEnclosingScope();
	void setEnclosingScope(const VariantReference<Scope>& scope);
	virtual bool equal(Variant*);
	static UserFunctionVariant* Create(int);
private:
	int address;
	VariantReference<Scope> enclosingScope;
	UserFunctionVariant(int address);
	
};
			
