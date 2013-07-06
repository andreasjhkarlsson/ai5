#include "FileFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\HandleVariant.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <functional>
#include <memory>
#include "..\AI5Runtime\encode.h"
#include "..\AI5Runtime\CallInfo.h"
using namespace std::placeholders;

FileFunctions::FileFunctions(void)
{
}


FileFunctions::~FileFunctions(void)
{
}


Variant* FileFunctions::printline(CallInfo* callInfo)
{

	for(int i=0;i<callInfo->getArgCount();i++)
	{
		if(i > 0)
			std::wcout << ", ";
		Variant* var = callInfo->getArg(i);
		var->format(std::wcout);
		
	}

	std::wcout << std::endl;

	NullVariant::Instance.addRef();
	return &NullVariant::Instance;
}


Variant* FileFunctions::fileExists(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	shared_string path = callInfo->getStringArg(0);


	return BooleanVariant::Get(PathFileExistsW(path->getTerminatedBuffer()) != 0,true);

}

void FileFunctions::registerFunctions(StackMachine* machine)
{
	std::shared_ptr<FileFunctions> instance(new FileFunctions);

	machine->addBuiltInFunction(L"printline",std::bind(&printline,instance,_1));
	machine->addBuiltInFunction(L"fileexists",std::bind(&fileExists,instance,_1));
	machine->addBuiltInFunction(L"consolewrite",std::bind(&consoleWrite,instance,_1));
	machine->addBuiltInFunction(L"consolewriteerror",std::bind(&consoleWriteError,instance,_1));
	machine->addBuiltInFunction(L"consolereadline",std::bind(&consoleReadLine,instance,_1));
	machine->addBuiltInFunction(L"filechangedir",std::bind(&fileChangeDir,instance,_1));
	machine->addBuiltInFunction(L"fileopen",std::bind(&fileOpen,instance,_1));
	machine->addBuiltInFunction(L"fileclose",std::bind(&fileClose,instance,_1));
	machine->addBuiltInFunction(L"fileread",std::bind(&fileRead,instance,_1));
	machine->addBuiltInFunction(L"filewrite",std::bind(&fileWrite,instance,_1));

}


Variant* FileFunctions::consoleWrite(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	std::wcout << callInfo->getStringArg(0)->getTerminatedBuffer();
	return nullptr;
}


Variant* FileFunctions::consoleWriteError(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	std::wcerr << callInfo->getStringArg(0)->getTerminatedBuffer();
	return nullptr;
}

Variant* FileFunctions::consoleReadLine(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,0);
	std::wstring str;
	std::getline(std::wcin,str);
	return new StringVariant(str.c_str());
}

Variant* FileFunctions::fileChangeDir(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	SetCurrentDirectoryW(callInfo->getStringArg(0)->getTerminatedBuffer());

	return nullptr;
}


class FileHandle: public HandleVariant
{
public:

	typedef int MODE;
	static const MODE READ = 1; 
	static const MODE WRITE = 2; // Implies READ
	static const MODE WRITE_APPEND = 4; // Implies READ

	static const HANDLE_TYPE HTYPE = FILE_HANDLE;
	FileHandle(shared_string filename,MODE mode,
		Encode::TYPE encoding,bool binary,bool bom,bool fullDetection):
			handle(nullptr), HandleVariant(FILE_HANDLE), filename(filename),
				mode(mode), encoding(encoding), binary(binary), bom(bom), fullDetection(fullDetection)
	{
		DWORD readWrite = 0;
		DWORD openOpt;
		if(mode&READ)
			readWrite |= GENERIC_READ;
		if(mode&WRITE || mode&WRITE_APPEND)
			readWrite |= GENERIC_WRITE;

		if(readWrite&WRITE_APPEND)
			openOpt = CREATE_NEW;
		else if(readWrite&WRITE)
			openOpt = CREATE_ALWAYS;
		else 
			openOpt = OPEN_EXISTING;

		handle = CreateFileW(filename->getTerminatedBuffer(),readWrite,0,NULL,openOpt,FILE_ATTRIBUTE_NORMAL,NULL);
		if(!handle)
		{
			// TODO: raise error!
			return;
		}

	}

	Variant* read(size_t count)
	{
		if(count == 0)
		{
			// TODO
		}


		//if(!binary && (encoding == Encode::UTF16_LITTLE_ENDIAN


	}

	virtual bool isValid()
	{
		return handle != nullptr;
	}
	virtual void cleanup()
	{
		close();
	}
	void close()
	{
		if(handle != nullptr)
		{
			CloseHandle(handle);
			handle = nullptr;
		}
	}
private:
	HANDLE handle;
	Encode::TYPE encoding;
	MODE mode;
	bool binary;
	bool bom;
	bool fullDetection;
	shared_string filename;

};


Variant* FileFunctions::fileOpen(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);


	shared_string filename = callInfo->getStringArg(0);
	int flag = callInfo->getInt32Arg(1,0);

	// Default values for flags.
	FileHandle::MODE mode = FileHandle::READ; 
	bool createDirectories = false;
	bool binary_mode = false; // Force every read to binary!
	bool bom = false; // Try to detect encoding with BOM.
	bool fullDetection = false; // Use entire file to detect possible UTF-8.
	Encode::TYPE encoding = Encode::CURRENT_CP;

	// Extract flags from bitfield.
	if(flag&0x1)
		mode = FileHandle::WRITE;
	if(flag&0x2)
		mode = FileHandle::WRITE_APPEND;
	if(flag&0x8)
		createDirectories = true;
	if(flag&0x10)
		binary_mode = true;
	if(flag&0x20)
	{
		encoding = Encode::UTF16_LITTLE_ENDIAN;
		bom = true;
	}
	if(flag&0x40)
	{
		encoding = Encode::UTF16_LITTLE_ENDIAN;
		bom = true;
	}

	if(flag&0x80)
	{
		encoding = Encode::UTF8;
		bom = true;
	}
	if(flag&0x100)
	{
		encoding = Encode::UTF8;
		bom = false;
	}
	if(flag&0x4000)
		fullDetection = true;

	return new FileHandle(filename,mode,encoding,binary_mode,bom,fullDetection);
	

}
Variant* FileFunctions::fileClose(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	callInfo->getHandleArg(0)->castHandle<FileHandle>()->close();

	return nullptr;
}
Variant* FileFunctions::fileRead(CallInfo* callInfo)
{

	return nullptr;
}
Variant* FileFunctions::fileWrite(CallInfo* callInfo)
{
	return nullptr;
}