#pragma once

#include "windows.h"
#include "variant.h"
#include "COMVar.h"
#include "types.h"

#define MAX_ARG_DLLCALL 32 // Maximum number of arguments for DllCall

class DllCall
{

public:
	DllCall                                              ();
	DllCall                                              (HMODULE, UnicodeString&, UnicodeString&, std::vector<UnicodeString>&);
	~DllCall                                             ();

	bool                          Invoke                 (std::vector<Variant*>&, COMVar*);
	__forceinline bool            SetFunc                (UnicodeString&);
	void                          SetFunc                (LPVOID);
	__forceinline void            SetRetTypeAndCC        (UnicodeString&);
	__forceinline void            SetParamsTypes         (std::vector<UnicodeString>&);

	// The size of the buffer in characters for dynamically allocated strings
	static const UINT             SIZE_BUFFER = 65536;

protected:
	VARTYPE                       VarType                (LPCWSTR);
	void                          FixDecimalSeparator    (BSTR, bool);
	PVOID                         BSTRToWSTR             (VARIANT*);
	PVOID                         BSTRToSTR              (VARIANT*);
	PVOID                         VARToCLSID             (VARIANT*);

private:
	PVOID                         VARByrefB              (VARIANT*);
	PVOID                         VARByrefW              (VARIANT*);
	PVOID                         VARByrefS              (VARIANT*);
	PVOID                         VARByrefV              (VARIANT*);
	PVOID                         VARByrefD              (VARIANT*);
	PVOID                         VARByrefU              (VARIANT*);
	PVOID                         VARByrefC              (VARIANT*);
	BSTR                          AllocReadStringByref   (VARIANT*, VARTYPE);
	BSTR                          AllocReadString        (VARIANT*, VARTYPE);
	bool                          VARCopyConvert         (VARIANT*, VARIANT*, VARTYPE);
	void                          VARDerefCopy           (VARIANT*, VARIANT*, VARTYPE);
	void                          CrackReturnType        (LPCWSTR, CALLCONV&, VARTYPE&);

	HMODULE                       hModule;
	CALLCONV                      cc;
	VARTYPE                       vtRetType;
	std::vector<VARTYPE>          vTypes;
	LPVOID                        pFunc;
};


