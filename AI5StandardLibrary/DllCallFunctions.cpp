#include "DllcallFunctions.h"
#include "..\AI5Runtime\DllCall.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\ListVariant.h"
#include "..\AI5Runtime\NullVariant.h"
#include "Handles.h"

using namespace std::placeholders;

DllCallFunctions::DllCallFunctions()
{
}


DllCallFunctions::~DllCallFunctions()
{
}



Variant* DllCallFunctions::dllcall(Variant** args, int argsSize)
{
	validateArgCount(argsSize, 3, 3 + 2 * MAX_ARG_DLLCALL);

	std::vector<std::wstring> vParamTypes;
	for (int i = 3; i < argsSize; ++i)
		vParamTypes.push_back(args[i++]->toString().get()->c_str()); // post-increment used to get every other element

	std::vector<Variant*> vArgs;
	for (int i = 4; i < argsSize; ++i)
		vArgs.push_back(args[i++]); // post-increment used to get every other element

	bool loadedModule = false;
	HMODULE hModule = nullptr;
	if(args[0]->isStringType())
	{
		hModule = ::LoadLibraryW(args[0]->toString()->c_str());
		loadedModule = true;
	}
	else
	{
		// How about this cast chain? :D If only C++ would allow virtual template methods...
		hModule = args[0]->cast<HandleVariant>()->castHandle<ModuleHandle>()->getModule();
	}

	// TODO: Check for hModule == nullptr and throw error.

	auto dllcall = DllCall(hModule, *args[1]->toString().get(), *args[2]->toString().get(), vParamTypes);

	// To collect processed arguments to (some may be altered byref)
	COMVar pcvRet[MAX_ARG_DLLCALL + 1];

	// Make the  actual call
	bool bRet = dllcall.Invoke(vArgs, pcvRet);

	// If we only loaded the module for this call.
	if(loadedModule)
		FreeLibrary(hModule);

	if (bRet == false)
		return nullptr; // TODO: Set error.

	ListVariant* vRet = new ListVariant;
	for (size_t i = 0; i < vArgs.size()+1; ++i)
	{
		vRet->addElement(Variant::createFromCOMVar(pcvRet[i]));
	}

	return vRet;
}

Variant* DllCallFunctions::dllcalladdress(Variant** args, int argsSize)
{
	validateArgCount(argsSize, 2, 3 + 2 * MAX_ARG_DLLCALL);

	std::vector<std::wstring> vParamTypes;
	for (int i = 2; i < argsSize; ++i)
		vParamTypes.push_back(args[i++]->toString().get()->c_str()); // post-increment used to get every other element

	std::vector<Variant*> vArgs;
	for (int i = 3; i < argsSize; ++i)
		vArgs.push_back(args[i++]); // post-increment used to get every other element

	auto dllcall = DllCall();

	dllcall.SetRetTypeAndCC(*args[0]->toString().get());
	dllcall.SetParamsTypes(vParamTypes);
	dllcall.SetFunc(reinterpret_cast<LPVOID>(args[1]->toInteger64())); // !!! Yes, that's right, it sucks! Do something. 

	// To collect processed arguments to (some may be altered byref)
	COMVar pcvRet[MAX_ARG_DLLCALL + 1];

	// Make the  actual call
	if (!!!dllcall.Invoke(vArgs, pcvRet))
		return nullptr; // TODO: Set error.

	ListVariant* vRet = new ListVariant;
	for (size_t i = 0; i < vArgs.size()+1; ++i)
	{
		vRet->addElement(Variant::createFromCOMVar(pcvRet[i]));
	}

	return vRet;
}

Variant* DllCallFunctions::dllopen(Variant** args, int argsSize)
{
	validateArgCount(argsSize, 1, 1);

	shared_string dllPath = args[0]->toString();

	HMODULE module = ::LoadLibraryW(dllPath->c_str());

	if(!module)
		return nullptr; // TODO: Set error.

	return new ModuleHandle(module);
}

Variant* DllCallFunctions::dllclose(Variant** args, int argsSize)
{
	validateArgCount(argsSize, 1, 1);

	ModuleHandle* handle = args[0]->cast<HandleVariant>()->castHandle<ModuleHandle>();
	handle->close();

	return nullptr;
}


void DllCallFunctions::registerFunctions(StackMachine* machine)
{

	std::shared_ptr<DllCallFunctions> instance(new DllCallFunctions);

	machine->addBuiltInFunction(L"dllcall", std::bind(&dllcall, instance, _1, _2));
	machine->addBuiltInFunction(L"dllcalladdress", std::bind(&dllcalladdress, instance, _1, _2));
	machine->addBuiltInFunction(L"dllopen", std::bind(&dllopen, instance, _1, _2));
	machine->addBuiltInFunction(L"dllclose", std::bind(&dllclose, instance, _1, _2));
}