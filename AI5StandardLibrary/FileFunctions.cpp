#include "FileFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\HandleVariant.h"
#include "..\AI5Runtime\IteratorVariant.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <functional>
#include <memory>
#include "..\AI5Runtime\encode.h"
#include "..\AI5Runtime\CallInfo.h"
#include "unicode/ustdio.h"
#include "unicode/uchar.h"
#include "unicode/ustring.h"
#include <string>
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
	machine->addBuiltInFunction(L"filereadline",std::bind(&fileReadLine,instance,_1));
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

		char* openMode;
		if(mode&READ)
			openMode = "r";
		if(mode&WRITE)
			openMode = "w";
		if(mode&WRITE_APPEND)
			openMode = "w+"; // <- This is not documented by ICU, but ICU uses regular fopen so it works!

		char* codepage = nullptr;
		if(encoding == Encode::UTF8)
			codepage = "UTF-8";
		else if(encoding == Encode::UTF16_LITTLE_ENDIAN)
			codepage = "UTF-16 LE";
		else if(encoding == Encode::UTF16_BIG_ENDIAN)
			codepage = "UTF-16 BE";

		std::string utf8_filename;
		// TODO: Use GetShortPath to make unicode filenames work.
		filename->toUTF8String(utf8_filename);
		handle = u_fopen(utf8_filename.c_str(),openMode,NULL,codepage);

		if(!handle)
		{
			// It's an error! Do something!
		}

	}

	Variant* read(size_t count)
	{
		int bytesRead;
		std::vector<UChar> buffer(count);
		bytesRead = u_file_read(&buffer[0],count,handle);

		return new StringVariant(create_shared_string(&buffer[0],bytesRead));
	}

	Variant* readLine()
	{
		shared_string str = create_shared_string(L"");
		const int buffer_size = 12;

		UChar buffer[buffer_size];
		int read_length;
		do
		{
			// If u_fgets returns nullptr, there are no more lines!
			if(!u_fgets(buffer,buffer_size,handle))
				break;
			read_length = u_strlen(buffer);

			// If the last char is '\n', then line has been processes.
			if(buffer[read_length-1] == L'\n')
			{
				// Do not append linebreak.
				str->append(buffer,read_length-1);
				break;
			}
			else
			{
				str->append(buffer,read_length);
			}
		}while(true);


		return new StringVariant(str);
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
		// You can close as many times as you like! :))
		if(handle != nullptr)
		{
			u_fclose(handle);
			handle = nullptr;
		}
	}

	bool isEOFReached()
	{
		return u_feof(handle);
	}

	class LineIterator: public IteratorVariant
	{
	public:
		LineIterator(FileHandle* handle): handle(handle)
		{
			handle->addRef();
		}
		virtual bool hasMore()
		{
			return !handle->isEOFReached();
		}
		virtual Variant* next()
		{
			return handle->readLine();
			
		}

		virtual void cleanup()
		{
			handle->release();
		}
	private:
		FileHandle* handle;
	};
	
	virtual IteratorVariant* iterate()
	{
		return new LineIterator(this);
	}


private:
	UFILE* handle;
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
	FileHandle::MODE mode; 
	bool createDirectories = false;
	bool binary_mode = false; // Force every read to binary!
	bool bom = false; // Try to detect encoding with BOM.
	bool fullDetection = false; // Use entire file to detect possible UTF-8.
	Encode::TYPE encoding = Encode::CURRENT_CP;

	// Extract flags from bitfield.
	if(flag&0x1)
		mode = FileHandle::WRITE;
	else if(flag&0x2)
		mode = FileHandle::WRITE_APPEND;
	else 
		mode = FileHandle::READ;
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
	callInfo->validateArgCount(1,2);

	int count = callInfo->getInt32Arg(1,0);

	return callInfo->getHandleArg(0)->castHandle<FileHandle>()->read(count);

}

Variant* FileFunctions::fileReadLine(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	return callInfo->getHandleArg(0)->castHandle<FileHandle>()->readLine();
}

Variant* FileFunctions::fileWrite(CallInfo* callInfo)
{
	return nullptr;
}