#include "DllcallFunctions.h"
#include "..\AI5Runtime\DllCall.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\ListVariant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\CallInfo.h"
#include "Handles.h"

using namespace std::placeholders;

DllCallFunctions::DllCallFunctions()
{
}


DllCallFunctions::~DllCallFunctions()
{
}



VariantReference<> DllCallFunctions::dllcall(CallInfo* callInfo)
{
	callInfo->validateArgCount(3, 3 + 2 * MAX_ARG_DLLCALL);

	std::vector<UnicodeString> vParamTypes;
	for (int i = 3; i < callInfo->getArgCount(); ++i)
		vParamTypes.push_back(callInfo->getStringArg(i++)->getTerminatedBuffer()); // post-increment used to get every other element

	std::vector<VariantReference<>> vArgs;
	for (int i = 4; i < callInfo->getArgCount(); ++i)
		vArgs.push_back(callInfo->getArg(i++)); // post-increment used to get every other element

	bool loadedModule = false;
	HMODULE hModule = nullptr;
	if(callInfo->getArg(0).isStringType())
	{
		hModule = ::LoadLibraryW(callInfo->getStringArg(0)->getTerminatedBuffer());
		loadedModule = true;
	}
	else
	{
		// How about this cast chain? :D If only C++ would allow virtual template methods...
		hModule = callInfo->getArg(0).cast<HandleVariant>()->castHandle<ModuleHandle>()->getModule();
	}

	// TODO: Check for hModule == nullptr and throw error.

	auto dllcall = DllCall(hModule, *callInfo->getArg(1).toString().get(),
		*callInfo->getStringArg(2), vParamTypes);

	// To collect processed arguments to (some may be altered byref)
	COMVar pcvRet[MAX_ARG_DLLCALL + 1];

	// Make the  actual call
	bool bRet = dllcall.Invoke(vArgs, pcvRet);

	// If we only loaded the module for this call.
	if(loadedModule)
		FreeLibrary(hModule);

	if (bRet == false)
		return nullptr; // TODO: Set error.

	ListVariant* vRet = ListVariant::Create();
	for (size_t i = 0; i < vArgs.size()+1; ++i)
	{
		VariantReference<> el = Variant::createFromCOMVar(pcvRet[i]);
		vRet->addElement(el);
	}

	return vRet;
}

VariantReference<> DllCallFunctions::dllcalladdress(CallInfo* callInfo)
{
	callInfo->validateArgCount(2, 3 + 2 * MAX_ARG_DLLCALL);

	std::vector<UnicodeString> vParamTypes;
	for (int i = 2; i < callInfo->getArgCount(); ++i)
		vParamTypes.push_back(callInfo->getStringArg(i++)->getTerminatedBuffer()); // post-increment used to get every other element

	std::vector<VariantReference<>> vArgs;
	for (int i = 3; i < callInfo->getArgCount(); ++i)
		vArgs.push_back(callInfo->getArg(i++)); // post-increment used to get every other element

	auto dllcall = DllCall();

	dllcall.SetRetTypeAndCC(*callInfo->getStringArg(0));
	dllcall.SetParamsTypes(vParamTypes);
	dllcall.SetFunc(callInfo->getArg(1).toPointer()); 

	// To collect processed arguments to (some may be altered byref)
	COMVar pcvRet[MAX_ARG_DLLCALL + 1];

	// Make the  actual call
	if (!!!dllcall.Invoke(vArgs, pcvRet))
		return nullptr; // TODO: Set error.

	ListVariant* vRet = ListVariant::Create();
	for (size_t i = 0; i < vArgs.size()+1; ++i)
	{
		VariantReference<> el = Variant::createFromCOMVar(pcvRet[i]);
		vRet->addElement(el);
	}

	return vRet;
}

VariantReference<> DllCallFunctions::dllopen(CallInfo* callInfo)
{
	callInfo->validateArgCount(1, 1);

	shared_string dllPath = callInfo->getStringArg(0);

	HMODULE module = ::LoadLibraryW(dllPath->getTerminatedBuffer());

	if(!module)
		return nullptr; // TODO: Set error.

	return GC::alloc<ModuleHandle,HMODULE>(module);
}

VariantReference<> DllCallFunctions::dllclose(CallInfo* callInfo)
{
	callInfo->validateArgCount(1, 1);

	ModuleHandle* handle = callInfo->getArg(0).cast<HandleVariant>()->castHandle<ModuleHandle>();
	handle->close();

	return nullptr;
}


void DllCallFunctions::registerFunctions(StackMachine* machine)
{

	std::shared_ptr<DllCallFunctions> instance(new DllCallFunctions);

	machine->addBuiltInFunction(L"dllcall", std::bind(&dllcall, instance, _1));
	machine->addBuiltInFunction(L"dllcalladdress", std::bind(&dllcalladdress, instance, _1));
	machine->addBuiltInFunction(L"dllopen", std::bind(&dllopen, instance, _1));
	machine->addBuiltInFunction(L"dllclose", std::bind(&dllclose, instance, _1));
}