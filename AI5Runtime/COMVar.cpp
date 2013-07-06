#include "COMVar.h"


// Default constructor
COMVar::COMVar()
{
	::VariantInit(this); // memory can be dirty and that's why VARIANT needs to be initialized
	this->ullVal = 0;    // zero-out the whole data field (redundant actually, but results in easier possible debugging)
}

// Destructor
COMVar::~COMVar()
{
	::VariantClear(this); // free any VARIANT within to free allocated memory space
	this->ullVal = 0;   // zero-out the whole data field (redundant actually, but results in easier possible debugging)
}

// Copy constructor
COMVar::COMVar(const COMVar& comVar)
{
	::VariantInit(this); // "this" VARIANT is not initialized yet when this is the case. In order to work with them, VARIANTs are to be initialized.
	if (FAILED(::VariantCopy(this, &comVar))) // copy passed VARIANT into "this" VARIANT. comVar is COMVar object.
	{
		// Do nothing.
	}
}

// Consrtuctor with pointer to COMVar
COMVar::COMVar(const COMVar* comVar)
{
	::VariantInit(this); // "this" VARIANT is not initialized yet when this is the case. In order to work with them, VARIANTs are to be initialized.
	if (FAILED(::VariantCopy(this, comVar))) // copy passed VARIANT into "this" VARIANT. comVar is pointer to COMVar.
	{
		// Do nothing.
	}
}

// Assignment operator with another COMVar.
void COMVar::operator= (const COMVar& comVar)
{
	::VariantClear(this); // release current content
	if (FAILED(::VariantCopy(this, &comVar))) // copy new VARIANT into "this" VARIANT
	{
		// Do nothing.
	}
}

// Assignment operator with pointer to another COMVar.
void COMVar::operator= (const COMVar* comVar)
{
	::VariantClear(this); // release current content
	if (FAILED(::VariantCopy(this, comVar))) // copy new VARIANT into "this" VARIANT
	{
		// Do nothing.
	}
}

// Assignment operator with Variant object.
void COMVar::operator= (const Variant& vVar)
{
	*this = COMVar(vVar); // Variant is turned into COMVar and then copyied into "this"
}

// Assignment operator with pointer to Variant.
void COMVar::operator= (const Variant* pVar)
{
	*this = COMVar(*pVar); // Pointer to Variant is dereferenced and get object is turned into COMVar and then copyied into "this"
}

// Constructor on Variant object
COMVar::COMVar(const Variant& vVar)
{
	::VariantInit(this);  // initialize "this" variant. Memory space can be dirty and that's why VARIANT needs to be initialized
	this->ullVal = 0;     // zero-out the whole data field (redundant actually, but results in easier possible debugging)

	// Handle different Variant-s
	switch (vVar.getType())
	{
	case Variant::BOOLEAN:
		this->vt		  =   VT_BOOL;
		this->boolVal	  =   vVar.toBoolean() ? VARIANT_TRUE : VARIANT_FALSE;
		break;

	case Variant::INTEGER32:
		this->vt		  =   VT_I4;
		this->lVal		  =   vVar.toInteger32();
		break;

	case Variant::FLOATING:
		this->vt		  =   VT_R8;
		this->dblVal	  =   vVar.toFloating();
		break;

	case Variant::INTEGER64:
		this->vt		  =   VT_I8;
		this->llVal	      =   vVar.toInteger64();
		break;

	case Variant::STRING:
		this->vt		  =  VT_BSTR;
		this->bstrVal	  =  ::SysAllocString(vVar.toString()->getTerminatedBuffer());
		break;

	case Variant::LIST:
		this->vt          =  VT_ARRAY | VT_VARIANT;
		this->parray      =  this->VariantListToSafeArray(vVar);
		break;

	case Variant::BINARY:
		this->vt = VT_ARRAY | VT_UI1;
		this->parray = this->VariantBinaryToSafeArray(vVar);
		break;

		//TODO: Add code for all other types

	default:
		// Do nothing
		break;
	}
}

// Constructor on pointer to Variant
COMVar::COMVar(const Variant* pVar)
{
	*this = COMVar(*pVar); // Pointer to Variant is dereferenced and get object is turned into COMVar and then copyied into "this"
}


///////////////////////////////////////////////////////////////////////////////
// COMVar::VariantBinaryToSafeArray()
//
// Converts Binary Variant into SafeArray and returns pointer to it
///////////////////////////////////////////////////////////////////////////////
SAFEARRAY* COMVar::VariantBinaryToSafeArray(const Variant& vVar)
{
	
	//TODO: Add implementation
	
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////
// COMVar::VariantListToSafeArray()
//
// Converts List Variant into SafeArray and returns pointer to it
//////////////////////////////////////////////////////////////////////////////////////
SAFEARRAY* COMVar::VariantListToSafeArray(const Variant& vVar)
{

	//TODO: Add implementation

	return nullptr;
}