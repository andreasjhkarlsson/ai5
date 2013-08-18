#include "FileFunctions.h"
#include "..\AI5Runtime\StackMachine.h"
#include "..\AI5Runtime\NullVariant.h"
#include "..\AI5Runtime\BooleanVariant.h"
#include "..\AI5Runtime\HandleVariant.h"
#include "..\AI5Runtime\IteratorVariant.h"
#include "..\AI5Runtime\BinaryVariant.h"
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


VariantReference<> FileFunctions::printline(CallInfo* callInfo)
{
	std::lock_guard<std::recursive_mutex> guard(printLock);
	for(int i=0;i<callInfo->getArgCount();i++)
	{
		if(i > 0)
			std::wcout << ", ";
		VariantReference<> var = callInfo->getArg(i);
		var.format(std::wcout);
		
	}

	std::wcout << std::endl;


	return nullptr;
}


VariantReference<> FileFunctions::fileExists(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	shared_string path = callInfo->getStringArg(0);


	return PathFileExistsW(path->getTerminatedBuffer()) != 0;

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


VariantReference<> FileFunctions::consoleWrite(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	std::wcout << callInfo->getStringArg(0)->getTerminatedBuffer();
	std::wcout.flush();
	return nullptr;
}


VariantReference<> FileFunctions::consoleWriteError(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);
	std::wcerr << callInfo->getStringArg(0)->getTerminatedBuffer();
	return nullptr;
}

VariantReference<> FileFunctions::consoleReadLine(CallInfo* callInfo)
{
	callInfo->validateArgCount(0,0);
	std::wstring str;
	std::getline(std::wcin,str);
	return StringVariant::Create(str.c_str());
}

VariantReference<> FileFunctions::fileChangeDir(CallInfo* callInfo)
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
	friend class GC;

	static const HANDLE_TYPE HTYPE = FILE_HANDLE;

	~FileHandle()
	{
		close();
	}

	int open(shared_string filename,MODE mode,
		Encode::TYPE encoding,bool binary,bool bom,bool fullDetection)
	{

		this->filename = filename;
		this->mode = mode;
		this->encoding = encoding;
		this->binary = binary;
		this->bom = bom;
		this->fullDetection = fullDetection;


		std::string openMode;
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

		if(binary)
		{
			openMode += "b";
			errno_t err = fopen_s(&handle,utf8_filename.c_str(),openMode.c_str());
			if(err)
				return 1;
		}
		else
		{

			uhandle = u_fopen(utf8_filename.c_str(),openMode.c_str(),nullptr,codepage);

			if(!uhandle)
			{
				return 1;
			}

		}

		if(encoding == Encode::UTF8 && bom)
		{
			char bom[3];

			fread(bom,1,3,u_fgetfile(uhandle));

		}

		return 0;
	}

	VariantReference<> read(size_t count)
	{
		if(binary)
		{

			if(count == 0)
			{
				// Calculate how many bytes to read.
				int curPos = ftell(handle);
				fseek(handle,0,SEEK_END);
				int endPos = ftell(handle);
				fseek(handle,SEEK_SET,curPos);

				count = endPos - curPos;
			}

			shared_binary binary = shared_binary(new std::vector<char>(count));
			fread(&(*binary)[0],1,count,handle);

			return BinaryVariant::Create(binary);
		}
		else
		{

			if(count == 0)
			{
				const size_t bufferSize = 1024;
				std::vector<UChar> buffer(bufferSize);
				shared_string str = create_shared_string(L"");

				while(!isEOFReached())
				{
					int charsRead = u_file_read(&buffer[0],bufferSize,uhandle);
					str->append(&buffer[0],charsRead);
				}

				return StringVariant::Create(str);
			}


			int charsRead;
			std::vector<UChar> buffer(count);
			charsRead = u_file_read(&buffer[0],count,uhandle);

			return StringVariant::Create(create_shared_string(&buffer[0],charsRead));
		}



	}

	VariantReference<StringVariant> readLine()
	{
		shared_string str = create_shared_string(L"");
		const int buffer_size = 12;

		UChar buffer[buffer_size];
		int read_length;
		do
		{

			// If u_fgets returns nullptr, there are no more lines!
			if(!u_fgets(buffer,buffer_size,uhandle))
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


		return StringVariant::Create(str);
	}

	virtual bool isValid() const
	{
		if(binary) return handle != nullptr;
		return uhandle != nullptr;
	}

	void close()
	{
		// You can close as many times as you like! :))
		if(handle != nullptr && binary)
		{
			fclose(handle);
			handle = nullptr;
		}
		if(uhandle != nullptr && !binary)
		{
			u_fclose(uhandle);
			uhandle = nullptr;
		}
	}

	bool isEOFReached()
	{
		if(binary)
			return feof(handle) != 0;
		return u_feof(uhandle) != 0;
	}

	class LineIterator: public IteratorVariant
	{
	public:
		LineIterator(const VariantReference<FileHandle>& handle): handle(handle)
		{

		}
		~LineIterator()
		{

		}
		virtual bool hasMore()
		{
			return !handle->isEOFReached();
		}
		virtual VariantReference<> next()
		{
			return handle->readLine();
			
		}
	private:
		VariantReference<FileHandle> handle;
	};
	
	virtual VariantReference<IteratorVariant> iterate()
	{
		return new LineIterator(this);
	}


private:
	FileHandle(): handle(nullptr), HandleVariant(FILE_HANDLE),uhandle(nullptr)
	{


	}
	UFILE* uhandle;
	FILE* handle;

	Encode::TYPE encoding;
	MODE mode;
	bool binary;
	bool bom;
	bool fullDetection;
	shared_string filename;

};


VariantReference<> FileFunctions::fileOpen(CallInfo* callInfo)
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

	VariantReference<FileHandle> fhandle = GC::alloc<FileHandle>();
	int status = fhandle->open(filename,mode,encoding,binary_mode,bom,fullDetection);
	if(status == 0)
		return fhandle;

	callInfo->setError(status);
	return -1;
	

}
VariantReference<> FileFunctions::fileClose(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	callInfo->getArg(0).cast<HandleVariant>()->castHandle<FileHandle>()->close();

	return nullptr;
}
VariantReference<> FileFunctions::fileRead(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,2);

	int count = callInfo->getInt32Arg(1,0);

	return callInfo->getArg(0).cast<HandleVariant>()->castHandle<FileHandle>()->read(count);

}

VariantReference<> FileFunctions::fileReadLine(CallInfo* callInfo)
{
	callInfo->validateArgCount(1,1);

	return callInfo->getArg(0).cast<HandleVariant>()->castHandle<FileHandle>()->readLine();
}

VariantReference<> FileFunctions::fileWrite(CallInfo* callInfo)
{
	return nullptr;
}