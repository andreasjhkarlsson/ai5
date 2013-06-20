#pragma once
#include "functionvariant.h"
#include "Scope.h"
class UserFunctionVariant :
	public FunctionVariant
{
public:
	UserFunctionVariant(int address);
	~UserFunctionVariant(void);
	std::wostream& format(std::wostream& stream);
	int getAddress();
	Scope* getEnclosingScope();
	void setEnclosingScope(Scope* scope);
	virtual void cleanup();
	virtual bool equal(Variant*);
private:
	int address;
	Scope* enclosingScope;
};
			
