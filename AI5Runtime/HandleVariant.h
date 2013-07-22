#pragma once
#include "variant.h"

typedef int HANDLE_TYPE;

class HandleVariant :
	public Variant
{
public:
	friend class GC;
	static const VARIANT_TYPE TYPE = HANDLE_VAR;

	static const HANDLE_TYPE MODULE_HANDLE = 0;
	static const HANDLE_TYPE FILE_HANDLE = 1;

	
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

	virtual bool isValid()const=0;
	virtual bool toBoolean() const override;
	virtual std::wostream& format(std::wostream& stream) const;
protected:
	HandleVariant(HANDLE_TYPE handleType);
private:
	HANDLE_TYPE handleType;
};

