#pragma once
#include <memory>
#include <string>
#include <vector>
#include "3rdparty\hsieh_hash.h"
#include "unicode/unistr.h"

class Variant;

struct NameIdentifier {
	int globalId;
	int localId;
	int staticId;
};

using icu::UnicodeString;

typedef std::shared_ptr<std::vector<char>> shared_binary;
//typedef std::shared_ptr<std::wstring> shared_string;
typedef std::shared_ptr<UnicodeString> shared_string;
typedef std::shared_ptr<std::vector<Variant*>> shared_var_list;
shared_string create_shared_string(const UnicodeString& string);
shared_string create_shared_string_from_wstring(const std::wstring& string);
shared_string create_shared_string(const wchar_t* str,int len);
UnicodeString int2string(int num);


typedef struct
{
	size_t operator() (const UnicodeString& k) const
	{
		return SuperFastHash((const char*)k.getBuffer(),k.length()/sizeof(UChar));
	}
} UnicodeStringHasher;
 
typedef struct
{
	bool operator() (const UnicodeString& x,const UnicodeString& y) const
	{
		return x.compare(y) == 0;
	}
} UnicodeStringComparator;


