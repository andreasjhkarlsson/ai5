#pragma once

#include "windows.h"
#include "variant.h"
#include "COMVar.h"

#define MAX_ARG_DLLCALL 32 // Maximum number of arguments for DllCall

class DllCall
{

public:
	DllCall                                              ();
	DllCall                                              (HMODULE, const std::wstring&, const std::wstring&, const std::vector<std::wstring>&);
	~DllCall                                             ();

	bool                          Invoke                 (const std::vector<Variant*>&, COMVar*);
	__forceinline bool            SetFunc                (const std::wstring&);
	void                          SetFunc                (LPVOID);
	__forceinline void            SetRetTypeAndCC        (const std::wstring&);
	__forceinline void            SetParamsTypes         (const std::vector<std::wstring>&);

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
	VOID                          VARDerefCopy           (VARIANT*, VARIANT*, VARTYPE);
	VOID                          CrackReturnType        (LPCWSTR, CALLCONV&, VARTYPE&);

	HMODULE                       hModule;
	CALLCONV                      cc;
	VARTYPE                       vtRetType;
	std::vector<VARTYPE>          vTypes;
	LPVOID                        pFunc;
};


