#pragma once
#include "variant.h"

typedef int HANDLE_TYPE;

class HandleVariant :
	public Variant
{
public:
	static const VARIANT_TYPE TYPE = HANDLE;

	static const HANDLE_TYPE MODULE_HANDLE = 0;

	HandleVariant(HANDLE_TYPE handleType);
	~HandleVariant(void);
	template<class T>
	T* castHandle()
	{
		if(handleType != T::HTYPE)
		{
			throw RuntimeError(L"Wrong handle type.");
		}
		return static_cast<T*>(this);
	}

	virtual bool isValid()=0;
	virtual bool toBoolean();

private:
	HANDLE_TYPE handleType;
};

