#pragma once

#include "windows.h"
#include "variant.h"

#ifndef _X64
// 64 bit code flag
#if defined(_M_X64) || defined(_WIN64)
#define _X64
#endif
#endif


class COMVar: public VARIANT
{
public:
	// Default constructor
	COMVar     ();
	// Destructor
	~COMVar    ();

	// Other constructor(s)
	COMVar     (const COMVar&);
	COMVar     (const COMVar*);
	COMVar     (const Variant&);
	COMVar     (const Variant*);

	// Assignment operator
	VOID operator= (const COMVar&);
	VOID operator= (const COMVar*);
	VOID operator= (const Variant&);
	VOID operator= (const Variant*);

	// Element types for sructures
#ifdef _X64
	static const VARTYPE VT_StructElemType = VT_UI8;
#else
	static const VARTYPE VT_StructElemType = VT_UI4;
#endif
	static const VARTYPE VT_StructType = VT_RECORD;

protected:
	SAFEARRAY*    VariantListToSafeArray       (const Variant&);
	SAFEARRAY*    VariantBinaryToSafeArray     (const Variant&);
};