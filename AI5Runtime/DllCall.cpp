#include "DllCall.h"
#include "COMVar.h"


// Default constructor
DllCall::DllCall(): bLoadedModule(false), hModule(nullptr), pFunc(nullptr), cc(CC_STDCALL), vtRetType(VT_INT)
{
}


// Constructor
DllCall::DllCall(const std::wstring& sModule, const std::wstring& sRetType, const std::wstring& sFunc, const std::vector<std::wstring>& sParamsTypes): bLoadedModule(false), hModule(nullptr), pFunc(nullptr), cc(CC_STDCALL), vtRetType(VT_INT)
{
	// Resolve ret type and calling convention
	this->SetRetTypeAndCC(sRetType.c_str());

	// Resolve params types
	this->SetParamsTypes(sParamsTypes);

	// Load the module
	if (sModule.size())
		this->hModule = ::LoadLibrary(sModule.c_str());

	// Get the function pointer
	if (this->hModule)
	{
		this->bLoadedModule = true;
		this->SetFunc(sFunc);
	}
}


// Destructor
DllCall::~DllCall()
{
	if (this->bLoadedModule)
		::FreeLibrary(this->hModule);
}


// Manually loads the module. Unloads previously loaded.
void DllCall::SetModule(const std::wstring& sModule)
{
	if (this->bLoadedModule)
	{
		::FreeLibrary(this->hModule);
		this->bLoadedModule = false;
	}
	if (sModule.size())
		this->hModule = ::LoadLibrary(sModule.c_str());
	if (this->hModule) // check if loaded now
		this->bLoadedModule = true;
}


// Manually sets preloaded module. Unloads previously loaded.
void DllCall::SetModule(HMODULE hMod)
{
	if (this->bLoadedModule)
		::FreeLibrary(this->hModule);
	this->hModule = hMod;
	this->bLoadedModule = false;
}


// Manually sets function pointer
void DllCall::SetFunc(LPVOID pFunction)
{
	this->pFunc = pFunction;
}


// Manually sets function
bool DllCall::SetFunc(const std::wstring&  sFunc)
{
	if (this->hModule)
	{
		std::string s(sFunc.begin(), sFunc.end());
		this->pFunc = ::GetProcAddress(this->hModule, s.c_str());
		// In case of failure try adding "A" to function name to mimic behaviour of AutoIt v3
		if (!!!this->pFunc)
			this->pFunc = ::GetProcAddress(this->hModule, s.append("A").c_str());
		return this->pFunc != nullptr;
	}
	return false;
}


// Manually loads the module. Unloads previously loaded.
void DllCall::SetRetTypeAndCC(const std::wstring& sRet)
{
	this->CrackReturnType(sRet.c_str(), &this->cc, &this->vtRetType);
}


// Manually loads the module. Unloads previously loaded.
void DllCall::SetParamsTypes(const std::vector<std::wstring>& sParams)
{
	for (size_t i = 0; i < sParams.size(); ++i)
		this->vTypes.push_back(this->VarType(sParams[i].c_str()));
}


// Calls function pointer. 
// First param is vector of Variant arguments and second one is array of COMVars whose elements are set by this function byref.
bool DllCall::Invoke(const std::vector<Variant*>& vArgs, COMVar* pcvResult)
{
	// Sanity check
	if (this->pFunc)
	{
		// Number of parameters
		int iFuncArgs = vArgs.size();

		// Number of parameters according to definition
		int iSpecFuncParams = this->vTypes.size();

		// Number of parameters must match number specified in definition
		if (iFuncArgs != iSpecFuncParams) return false;

		// Number of parameters to send to function.
		// Initially this is the same as number of passed paarameters.
		int iFuncParams = iFuncArgs;
		int iIndex = -1; // initial index of processed parameter


		/************************************************************
		*
		* Correction in number of calling method parameters
		*
		************************************************************/
		// This requires going through all passed parameters in order to find passed structures.
		// If structure is found number of parameters for the function to call will be increased for the number of
		// array elements of converted structure.
		for (int i = 0; i < iFuncArgs; ++i)
		{
			// Check if "struct" type by definition
			if (this->vTypes[i] == COMVar::VT_StructType)
			{
				// TODO: Add implementation
			}
		}

		// Maximum number of arguments must not be exceeded
		if (iFuncArgs > MAX_ARG_DLLCALL)
			return false;

		/************************************************************
		*
		* Arrays allocations. On stack for speed.
		*
		************************************************************/
		VARTYPE prgvt[MAX_ARG_DLLCALL]; // array of vartypes (for DispCallFunc)
		VARIANT* prgpvarg[MAX_ARG_DLLCALL]; // array of variant pointers (for DispCallFunc)

		COMVar pvSubVariant[MAX_ARG_DLLCALL]; // array of sub-variants
		PVOID pSubBuffer[MAX_ARG_DLLCALL]; // array of pointers (to store pointers for buffers to create)

		int pIndexes[MAX_ARG_DLLCALL]; // array of parameters' indexes

		COMVar vCurrentParam;

		for (int i = 0; i < iFuncArgs; ++i)
		{
			++iIndex; // next parameter
			pIndexes[i] = iIndex; // store argument's index

			// Preset initial values
			prgvt[iIndex] = VT_ILLEGAL;
			prgpvarg[iIndex] = nullptr;
			pSubBuffer[iIndex] = nullptr; // in case no sub-allocation would be done (calling delete on it later)

			// Currently processed method parameter
			// TODO: Check for possible references
			vCurrentParam = vArgs[i];

			// Parameter type by Interface specification
			VARTYPE vParamType = this->vTypes[i];
			// Basic type out of that
			VARTYPE vParamTypeInd = vParamType & VT_TYPEMASK;


			/************************************************************
			*
			* Copy and convert input parameters to match specification
			*
			************************************************************/
			{
				// "struct" or "struct*" types
				if ((vParamType == COMVar::VT_StructType) || (vParamType == (COMVar::VT_StructType | VT_BYREF)))
				{
					// TODO: Add implementation
				}
				// All other types
				else if (this->VARCopyConvert(&vCurrentParam, &pvSubVariant[iIndex], vParamTypeInd) == FALSE)
				{
					// "clsid" type
					if (vParamTypeInd == VT_CLSID)
					{
						// Check for byref flag
						(vParamType & VT_BYREF) ? vParamType = VT_BYREF : vParamType = VT_EMPTY;
						// Set "new" type for the param
#ifdef _X64
						vParamType += VT_UI8;
#else
						vParamType += VT_UI4;
#endif
						// Process input, allocate CLSID
						pSubBuffer[iIndex] = this->VARToCLSID(&pvSubVariant[iIndex]);
					}

					else
					{
						// In case of failure clear the VARIANT...
						::VariantClear(&pvSubVariant[iIndex]);
						pvSubVariant[iIndex].ullVal = 0;  // this clears the whole data field regardless of type of VARIANT. May not be necessarry but just in case.
						pvSubVariant[iIndex].vt = vParamType; // ...and set/restore the type field
					}
				}

				// "wstr" and "str" strings need special attention
				if (vParamType == VT_LPWSTR)
				{
					vParamType = VT_BSTR; // this type correction is because of how DispCallFunc interprets strings
					// Convert VARIANT and save allocated string buffer to release aferwards
					pSubBuffer[iIndex] = this->BSTRToWSTR(&pvSubVariant[iIndex]);
				}
				else if (vParamType == VT_LPSTR)
				{
					vParamType = VT_BSTR; // this type correction is because of how DispCallFunc interprets strings
					// Convert VARIANT and save allocated string buffer to release aferwards
					pSubBuffer[iIndex] = this->BSTRToSTR(&pvSubVariant[iIndex]);
				}
			}


			/************************************************************
			*
			* Process byref parameters
			*
			************************************************************/
			{
				// Some parameters need to be passed by reference. That means that every VARIANT parameter must be of VT_BYREF type.
				// These VARIANTs require valid memory buffers that they point to. Proper VARIANT may not be passed if e.g. constants are used for parameters to calls from scripts.
				if(vParamType & VT_BYREF)
				{
					// Process different data types
					switch (vParamTypeInd)
					{
					case VT_VARIANT:
						pSubBuffer[iIndex] = this->VARByrefV(&pvSubVariant[iIndex]);
						break;
					case VT_BSTR:
						pSubBuffer[iIndex] = this->VARByrefB(&pvSubVariant[iIndex]);
						break;
					case VT_LPWSTR:
						pSubBuffer[iIndex] = this->VARByrefW(&pvSubVariant[iIndex]);
						break;
					case VT_LPSTR:
						pSubBuffer[iIndex] = this->VARByrefS(&pvSubVariant[iIndex]);
						break;
					case VT_DISPATCH:
						pSubBuffer[iIndex] = this->VARByrefD(&pvSubVariant[iIndex]);
						break;
					case VT_CLSID:
						pSubBuffer[iIndex] = this->VARByrefC(&pvSubVariant[iIndex]);
						break;
					default:
						if (pSubBuffer[iIndex] == nullptr) pSubBuffer[iIndex] = this->VARByrefU(&pvSubVariant[iIndex]);
					}

					// Sub-allocations are done. What's left now is clear current vSubVariant and fill it with new data.
					// First clear passed
					::VariantClear(&pvSubVariant[iIndex]);

					// And then set new and correct data according to Interface definition
					pvSubVariant[iIndex].vt = vParamType;
					pvSubVariant[iIndex].byref = pSubBuffer[iIndex];
				}
			}


			/************************************************************
			*
			* Fill arrays for DispCallFunc (if not already)
			*
			************************************************************/
			// Set type in array of types to current type
			if (prgvt[iIndex] == VT_ILLEGAL) prgvt[iIndex] = vParamType;
			// Set pointer in array of pointers to current VARIANT
			if (prgpvarg[iIndex] == nullptr) prgpvarg[iIndex] = &pvSubVariant[iIndex];
		}

		/************************************************************
		*
		* Ret VARIANT predprocessing
		*
		************************************************************/
		COMVar vRet; // VARIANT to collect the result

		VARTYPE vtRet = this->vtRetType;
		// Correct type of ret variant (these are all pointers):
		if ((vtRet & VT_BYREF) || (vtRet == VT_LPSTR) || (vtRet == VT_LPWSTR) || (vtRet == VT_CLSID))
		{
			vtRet = VT_UI8; // for pointer to fit in any case. Eiher for x64 or x86.
		}

		/************************************************************
		*
		* Call the function using DispCallFunc
		*
		************************************************************/
		HRESULT hr = ::DispCallFunc(nullptr,
			reinterpret_cast<ULONG_PTR>(this->pFunc), // function pointer
			this->cc, // calling convention
			vtRet, //type of return variant
			static_cast<UINT>(iFuncParams), // number of parameters
			prgvt, // pointer to array of param types
			prgpvarg, // pointer to array of variant pointers
			&vRet); // pointer to ret variant


		/************************************************************
		*
		* Process the result
		*
		************************************************************/
		{
			// Type of result according to documentation
			VARTYPE vRetType = this->vtRetType;

			if ((vRetType & VT_BYREF) && vRet.byref)
			{
				// Basic type of ret variant by Interface definition
				VARTYPE vTypeInd = vRetType & VT_TYPEMASK; // basic type
				// Setting correct type of ret variant (according to Interface description)
				vRet.vt = vRetType;
				// Copy dereferenced value of VARIANT
				this->VARDerefCopy(&vRet, &pcvResult[0], vTypeInd);
			}
			else
			{
				// Deal with special cases that Variant class can't
				switch (vRetType)
				{
				case VT_LPWSTR:
				case VT_LPSTR:
				case VT_CLSID:
					{
						// Set new values. These are strings...
						pcvResult[0].vt = VT_BSTR;
						pcvResult[0].bstrVal = this->AllocReadString(&vRet, vRetType);
					}
					break;
				default:
					// Copy found
					if (FAILED(::VariantCopy(&pcvResult[0], &vRet)))
					{
						pcvResult[0].ullVal = 0; // clear data field completely, something is wrong.
					}
				}

			}
		}

		/************************************************************
		*
		* Collect new values of byref-ed parameters
		*
		************************************************************/
		{
			for (int i = 0; i < iFuncArgs; ++i)
			{
				// Copy back parameters passed by reference
				{
					// Type of parameter by description
					VARTYPE& vParamType = this->vTypes[i];
					// Currently processed method parameter...
					if ((vParamType & VT_BYREF) && pvSubVariant[pIndexes[i]].byref)
					{
						// Basic parameter type out of the type
						VARTYPE vParamTypeInd = vParamType & VT_TYPEMASK;

						// Copy dereferenced value of VARIANT
						this->VARDerefCopy(&pvSubVariant[pIndexes[i]], &pcvResult[i + 1], vParamTypeInd);

						// Releasing memory for when special allocation is done per parameter.
						if (vParamTypeInd == VT_VARIANT)
						{
							VARIANT* vAlloc = static_cast<VARIANT*>(pSubBuffer[pIndexes[i]]);
							if (vAlloc) ::VariantClear(vAlloc); // in case VARIANT was allocated
						}
					}
					else if ((vParamType == VT_LPWSTR) || (vParamType == VT_LPSTR)) // these type of strings are in fact pointers. By reference allways.
					{
						//... and set new values
						pcvResult[i + 1].vt = VT_BSTR;
						pcvResult[i + 1].bstrVal = this->AllocReadString(&pvSubVariant[pIndexes[i]], vParamType);
					}
					else
						pcvResult[i + 1] = vArgs[i];
				}
			}
		}


		// Return whatever the result
		return hr == S_OK;

	}

	//... or return failure
	return false;

}

// Copies input variant (byref type) into output variant depending on specified type.
void DllCall::VARDerefCopy(VARIANT* vInput, VARIANT* vOutput, VARTYPE vType)
{
	// "wstr" and "str" types require manual conversion. VariantCopyInd can't handle them because it doesn't know what method is used for allocation.
	// On the other hand "bstr" needs clearing the original BSTR.
	// "clsid" is string type in script.
	switch (vType)
	{
	case VT_BSTR:
	case VT_LPWSTR:
	case VT_LPSTR:
	case VT_CLSID:
		// Clear byref-ed parameter...
		::VariantClear(vOutput);
		//... and set new values
		vOutput->vt = VT_BSTR;
		vOutput->bstrVal = this->AllocReadStringByref(vInput, vType);
		break;
	default:
		if (FAILED(::VariantCopyInd(vOutput, vInput)))
		{
			// VARIANT can be of type VT_VARIANT.
			if (vType == VT_VARIANT)
			{
				VARIANT *vVarBelow = vInput->pvarVal;
				VARTYPE vTypeBelow = vVarBelow->vt;
				::VariantClear(vOutput);
				// Process different types
				switch (vTypeBelow)
				{
				case VT_BSTR:
				case VT_LPWSTR:
				case VT_LPSTR:
				case VT_CLSID:
					// These are strings
					vOutput->vt = VT_BSTR;
					vOutput->bstrVal = this->AllocReadString(vVarBelow, vTypeBelow);
					break;
				default:
					// Just copy what's found.
					vOutput->vt = vTypeBelow;
					vOutput->byref = vVarBelow->byref;
				}
			}
			else
			{
				// Do nothing.
			}
		}
	}
}


// Copies input variant into output variant. Converts output variant to specified type.
bool DllCall::VARCopyConvert(VARIANT* vInput, VARIANT* vOutput, VARTYPE vType)
{
	// Copy first
	if (FAILED(::VariantCopy(vOutput, vInput))) 
		return false;

	// VT_VARIANT type can be any type. No need to do any conversion.
	if (vType == VT_VARIANT) 
		return true;

	// If the type is "wstr" or "str" then still go for BSTR as that's the type of input. Otherwise VariantChangeType will fail.
	switch (vType)
	{
	case VT_LPWSTR:
	case VT_LPSTR:
		vType = VT_BSTR;
	}

	// Check if conversion is required at all
	if (vInput->vt == vType) 
		return true;

	// If input is string variant (VT_BSTR) then fixing decimal separator is required. Decimal separator is dot and COM's is comma.
	if (vInput->vt == VT_BSTR) this->FixDecimalSeparator(vOutput->bstrVal, false); // dot to comma

	HRESULT hr = ::VariantChangeType(vOutput, vOutput, 0, vType);

	// Fix decimal separator
	if (vOutput->vt == VT_BSTR) 
		this->FixDecimalSeparator(vOutput->bstrVal, true); // comma to dot

	if (SUCCEEDED(hr)) 
		return true; // All Right!

	// If here conversion failed
	return false;
}


// Converts VT_BSTR to VT_LPWSTR in place returning the address of allocated memory for the string
PVOID DllCall::BSTRToWSTR(VARIANT* vVar)
{
	PVOID wString = this->VARByrefW(vVar);

	// Clear current content...
	::VariantClear(vVar);

	// ...and set new
	vVar->vt = VT_LPWSTR;
	vVar->byref = wString;

	return wString;
}


// Converts VT_BSTR to VT_LPSTR in place returning the address of allocated memory for the string
PVOID DllCall::BSTRToSTR(VARIANT* vVar)
{
	PVOID sString = this->VARByrefS(vVar);

	// Clear current...
	::VariantClear(vVar);

	// ...and set new
	vVar->vt = VT_LPSTR;
	vVar->byref = sString;

	return sString;
}


// Converts input VARIANT to an appropriate CLSID type in place, returning the address of allocated memory for the CLSID
PVOID DllCall::VARToCLSID(VARIANT* vVar)
{
	PVOID pCLSID = this->VARByrefC(vVar);

	// Clear current...
	::VariantClear(vVar);

	// ...and set new
#ifdef _X64
	vVar->vt = VT_UI8;
#else
	vVar->vt = VT_UI4;
#endif
	vVar->byref = pCLSID;

	return pCLSID;
}


// Allocates memory for VT_CLSID and copies data from passed variant into it
PVOID DllCall::VARByrefC(VARIANT* vVar) // Byref CLSID
{
	CLSID* pCLSID = new CLSID[1];
	// "clsid" in script will be either string representation or pointer to CLSID (struct). Third option coud be if it's passed just as dllstruct.
	if (vVar->vt == VT_BSTR) // if string representation of CLSID then convert to real CLSID
		::CLSIDFromString(vVar->bstrVal, pCLSID);
	else if (vVar->vt == COMVar::VT_StructType) // possibly struct
	{
		// TODO: Add implementation
	}
	else // if not string then it's pointer to CLSID, copy it...
		*pCLSID = *static_cast<CLSID*>(vVar->byref);

	return pCLSID;
}


// Allocates memory for VT_LPSTR and copies the string data from passed variant into it after convering it to ANSI
PVOID DllCall::VARByrefS(VARIANT* vVar) // Byref LPSTR
{
	// Read the string
	BSTR bString = vVar->bstrVal;
	UINT iLen = ::SysStringLen(bString); // the size of it

	UINT iAllocSize = this->SIZE_BUFFER;
	if (iLen > iAllocSize) 
		iAllocSize = iLen;

	// Allocate new string in that size (plus space for null-terminator)
	LPSTR pSubBuffer = new CHAR[iAllocSize + 1];

	// Convert to ANSI string
	::WideCharToMultiByte(CP_ACP, 0, static_cast<LPWSTR>(bString), -1, pSubBuffer, iLen + 1, nullptr, nullptr);

	return pSubBuffer;
}


// Allocates memory for VT_LPWSTR and copies the string data from passed variant into it
PVOID DllCall::VARByrefW(VARIANT* vVar) // Byref LPWSTR
{
	// Read the string
	BSTR bString = vVar->bstrVal;
	UINT iLen = ::SysStringLen(bString); // the size of it

	UINT iAllocSize = this->SIZE_BUFFER;
	if (iLen > iAllocSize) iAllocSize = iLen;

	// Allocate new wide-string in that size
	LPWSTR pSubBuffer = new WCHAR[iAllocSize + 1];

	// Copy the string to it
	if (bString) 
		::lstrcpyW(pSubBuffer, static_cast<LPWSTR>(bString));
	pSubBuffer[iLen] = '\0'; //  null-terminator

	return pSubBuffer;
}


// Allocates memory for VT_BSTR and copies the string data from passed variant into it
PVOID DllCall::VARByrefB(VARIANT* vVar) // Byref BSTR
{
	BSTR* bString = new BSTR[1];
	*bString = vVar->bstrVal;
	return bString;
}


// Allocates memory for VT_VARIANT and copies the variant data from passed variant into it
PVOID DllCall::VARByrefV(VARIANT* vVar) // Byref VARIANT
{
	// Allocate new buffer.
	VARIANT* pSubBuffer = new VARIANT[1];

	// Initialize this particular VARIANT
	::VariantInit(pSubBuffer);

	// Collect/copy passed for [in, out] parameters
	if (FAILED(::VariantCopy(pSubBuffer, vVar)))
		vVar->ullVal = 0;  // this clears the whole data field. Considering the failure this is necessary to have "clear" memory.

	return pSubBuffer;
}


// Allocates memory for VT_DISPATCH and copies the dispatch data from passed variant into it
PVOID DllCall::VARByrefD(VARIANT* vVar) // Byref Dispatch
{
	// Allocate new buffer.
	IDispatch** pSubBuffer = new IDispatch*[1];

	// Two cases here. One is if object is passed...
	if (vVar->vt == VT_DISPATCH)
	{
		*pSubBuffer = vVar->pdispVal; // that's passed value
		// Add ref count if it's valid object
		if (vVar->pdispVal != nullptr) vVar->pdispVal->AddRef();
	}
	//...and the other is if anything else is passed under object.
	else
		// Do nothing. How to confirm validity of the object?
		*pSubBuffer = nullptr;
	return pSubBuffer;
}


// Allocates memory for basic VARIANTs and copies the data from passed variant into it
PVOID DllCall::VARByrefU(VARIANT* vVar) // Byref anything that fits the size of ULONGLONG
{
	// Allocate new buffer.
	PVOID pSubBuffer = new ULONGLONG[1];

	// Read current value. This reads the whole data field of the VARIANT even though the whole size of it may not be set (union).
	// It shouldn't matter because vt field is set to correct value later.
	ULONGLONG pVal = vVar->ullVal;

	// Copy passed data for [in, out] parameters
	::memcpy(pSubBuffer, &pVal, sizeof ULONGLONG);

	return pSubBuffer;
}


// Allocates BSTR from string VARIANTs passed byref
BSTR DllCall::AllocReadStringByref(VARIANT* vVar, VARTYPE vType)
{
	BSTR bStringOut = nullptr; // preset the output

	if (vType == VT_LPWSTR)
	{
		LPWSTR wString = nullptr;
		// Read byref-ed value
		if(vVar->byref) wString = *static_cast<LPWSTR*>(vVar->byref);
		bStringOut = ::SysAllocString(wString);
	}
	else if (vType == VT_LPSTR)
	{
		// Read byref-ed value
		LPSTR sString = *static_cast<LPSTR*>(vVar->byref);

		// Calculate lengh of the read string
		int iLen = ::MultiByteToWideChar(CP_ACP, 0, sString, -1, nullptr, 0) ;
		// Allocate the memory for temporary string in that size
		LPWSTR wStringOut = new WCHAR[iLen];

		// Convert/copy to wide-string
		::MultiByteToWideChar(CP_ACP, 0, sString, -1, wStringOut, iLen);

		bStringOut = ::SysAllocString(wStringOut);

		// Clear temporary string
		delete [] wStringOut;
	}
	else if (vType == VT_BSTR)
	{
		// Read byref-ed value
		BSTR bString = *vVar->pbstrVal;

		bStringOut = ::SysAllocString(bString);

		// Clear the original
		::SysFreeString(bString);
	}
	else if (vType == VT_CLSID)
	{
		// Read new CLSID value
		CLSID* pCLSID = static_cast<CLSID*>(vVar->byref);

		// Convert GUID to a string
		OLECHAR sCLSID[40];
		if (::StringFromGUID2(*pCLSID, sCLSID, 40))
			bStringOut = ::SysAllocString(sCLSID);
	}

	return bStringOut;
}


// Allocates BSTR from string VARIANTs
BSTR DllCall::AllocReadString(VARIANT* vVar, VARTYPE vType)
{
	BSTR bStringOut = nullptr; // preset the output

	if (vType == VT_LPWSTR)
	{
		LPWSTR wString = nullptr;
		// Read byref-ed value
		if(vVar->byref) 
			wString = static_cast<LPWSTR>(vVar->byref);
		bStringOut = ::SysAllocString(wString);
	}
	else if (vType == VT_LPSTR)
	{
		// Read byref-ed value
		LPSTR sString = static_cast<LPSTR>(vVar->byref);

		// Calculate lengh of the read string
		int iLen = ::MultiByteToWideChar(CP_ACP, 0, sString, -1, nullptr, 0) ;
		// Allocate the memory for temporary string in that size
		LPWSTR wStringOut = new WCHAR[iLen];

		// Convert/copy to wide-string
		::MultiByteToWideChar(CP_ACP, 0, sString, -1, wStringOut, iLen);

		bStringOut = ::SysAllocString(wStringOut);

		// Clear temporary string
		delete [] wStringOut;
	}
	else if (vType == VT_BSTR)
	{
		// Read byref-ed value
		BSTR bString = vVar->bstrVal;

		bStringOut = ::SysAllocString(bString);

		// Clear the original
		::SysFreeString(bString);
	}
	else if (vType == VT_CLSID)
	{
		// Read byref-ed value
		CLSID pCLSID = *static_cast<CLSID*>(vVar->byref);

		// Convert GUID to a string
		OLECHAR sCLSID[40];
		if (::StringFromGUID2(pCLSID, sCLSID, 40))
			bStringOut = ::SysAllocString(sCLSID);
	}

	return bStringOut;
}


// Replaces decimal separator inside a BSTR string. If bFlag is true comma is turned to dot and if bFlag is false dot to comma
void DllCall::FixDecimalSeparator(BSTR bString, bool bFlag)
{
	LPWSTR wString = static_cast<LPWSTR>(bString);
	int    i       = 0;

	while (wString[i]) // until null-terminator encountered
	{
		if (bFlag)
		{
			if (wString[i] == ',') 
				wString[i] = '.'; // from comma to dot
		}
		else
		{
			if (wString[i] == '.') 
				wString[i] = ','; // from dot to comma
		}
		++i;
	}
}


// Cracks string describing the vt of the return VARIANT
void  DllCall::CrackReturnType(LPCWSTR sType, CALLCONV* cConv, VARTYPE* vRetType)
{
	*cConv = CC_STDCALL; // preset

	// Determine the type of ret VARIANT
	*vRetType = this->VarType(sType);

	// If valid type then return
	if (*vRetType != VT_ILLEGAL) 
		return; // All done!

	// In case calling convention was specified...
	int iLen = ::lstrlenW(sType);
	LPWSTR sTypeLocal = new WCHAR[iLen + 1];
	::lstrcpyW(sTypeLocal, sType);
	sTypeLocal[iLen] = '\0';

	int i = 0; // currently processed character inside the string
	while (sTypeLocal[i]) // until null-terminator encountered
	{
		if (sTypeLocal[i] == ':')
		{
			LPWSTR sConvention = static_cast<LPWSTR>(&sTypeLocal[i + 1]); // after colon character there should be calling convention specified
			// Can be cdecl:
			LPCWSTR sCdecl = L"cdecl";
			if (::lstrcmpiW(sConvention, sCdecl) == 0) 
				*cConv = CC_CDECL;

			sTypeLocal[i] = '\0'; // null terminate after the type
			*vRetType = this->VarType(sTypeLocal);
			// e.g. "dword*:cdecl" or "dword:cdecl"
			break; // all done
		}
		++i;
	}

	// Free allocated memory
	delete [] sTypeLocal;
}


// Returns VARTYPE of passed type identifier
VARTYPE DllCall::VarType(LPCWSTR sVarType)
{
	// Array of type identifiers
	static LPWSTR sTypeArray[] = {
		L"none",                                                 // 0
		L"byte",                                                 // 1
		L"boolean", L"bool",                                     // 2, 3
		L"short",                                                // 4
		L"ushort", L"word",                                      // 5, 6
		L"dword", L"ulong", L"uint",                             // 7, 8, 9
		L"long", L"int",                                         // 10, 11
		L"variant",                                              // 12
		L"int64",                                                // 13
		L"uint64",                                               // 14
		L"float",                                                // 15
		L"double",                                               // 16
		L"hresult",                                              // 17
		L"str",                                                  // 18
		L"wstr",                                                 // 19
		L"bstr",                                                 // 20
		L"ptr",                                                  // 21
		L"handle", L"hwnd",                                      // 22, 23
		L"int_ptr", L"long_ptr", L"lresult", L"lparam",          // 24, 25, 26, 27
		L"uint_ptr", L"ulong_ptr", L"dword_ptr", L"wparam",      // 28, 29, 30, 31
		L"idispatch", L"object",                                 // 32, 33
		L"struct",                                               // 34
		L"clsid"                                                 // 35
	};

	// Array of matching types
	static VARTYPE VarTypeArray[] = {
		VT_EMPTY,                              // none           0
		VT_UI1,                                // byte           1
		VT_BOOL,                               // boolean        2
		VT_BOOL,                               // bool           3
		VT_I2,                                 // short          4
		VT_UI2,                                // ushort         5
		VT_UI2,                                // word           6
		VT_UI4,                                // dword          7
		VT_UI4,                                // ulong          8
		VT_UI4,                                // uint           9
		VT_I4,                                 // long           10
		VT_I4,                                 // int            11
		VT_VARIANT,                            // variant        12
		VT_I8,                                 // int64          13
		VT_UI8,                                // uint64         14
		VT_R4,                                 // float          15
		VT_R8,                                 // double         16
		VT_I4,                                 // hresult        17
		VT_LPSTR,                              // str            18
		VT_LPWSTR,                             // wstr           19
		VT_BSTR,                               // bstr           20
#ifdef _X64
		VT_UI8,                                // ptr            21
		VT_UI8,                                // handle         22
		VT_UI8,                                // hwnd           23
		VT_I8,                                 // int_ptr        24
		VT_I8,                                 // long_ptr       25
		VT_I8,                                 // lresult        26
		VT_I8,                                 // lparam         27
		VT_UI8,                                // uint_ptr       28
		VT_UI8,                                // ulong_ptr      29
		VT_UI8,                                // dword_ptr      30
		VT_UI8,                                // wparam         31
#else
		VT_UI4,                                // ptr            21
		VT_UI4,                                // handle         22
		VT_UI4,                                // hwnd           23
		VT_I4,                                 // int_ptr        24
		VT_I4,                                 // long_ptr       25
		VT_I4,                                 // lresult        26
		VT_I4,                                 // lparam         27
		VT_UI4,                                // uint_ptr       28
		VT_UI4,                                // ulong_ptr      29
		VT_UI4,                                // dword_ptr      30
		VT_UI4,                                // wparam         31
#endif
		VT_DISPATCH,                           // idispatch      32
		VT_DISPATCH,                           // object         33
		COMVar::VT_StructType,                 // struct         34
		VT_CLSID                               // clsid          35
	};

	int iBound = sizeof(sTypeArray) / sizeof(LPVOID);
	VARTYPE vRet = VT_EMPTY;
	bool fFound = false;

	int iLen = ::lstrlenW(sVarType);
	LPWSTR sType = new WCHAR[iLen + 1];
	::lstrcpyW(sType, sVarType);
	sType[iLen] = '\0';

	if ((sType[iLen - 1] == '*') || (sType[iLen - 1] == '^'))
	{
		sType[iLen - 1] = '\0'; // replacing asterisk/caret with null
		vRet |=  VT_BYREF; // add "byref" flag
	}

	// For every element...
	for(int i = 0; i < iBound; ++i)
	{
		if (::lstrcmpiW(sTypeArray[i], sType) == 0) // match!
		{
			vRet += VarTypeArray[i]; // add type
			fFound = true;
			break;
		}
	}

	delete [] sType; // free allocated memory

	// If there is a match return the type
	if (fFound) 
		return vRet;

	// No match if here
	return VT_ILLEGAL;
}

