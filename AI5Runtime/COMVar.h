#pragma once

#include "windows.h"
#include "variant.h"
#include "VariantReference.h"

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
	COMVar     (const VariantReference<>&);

	// Assignment operator
	void operator= (const COMVar&);
	void operator= (const COMVar*);
	void operator= (const VariantReference<>&);

	// Element types for sructures
#ifdef _X64
	static const VARTYPE VT_StructElemType = VT_UI8;
#else
	static const VARTYPE VT_StructElemType = VT_UI4;
#endif
	static const VARTYPE VT_StructType = VT_RECORD;

protected:
	SAFEARRAY*    VariantListToSafeArray       (const VariantReference<>&);
	SAFEARRAY*    VariantBinaryToSafeArray     (const VariantReference<>&);
};