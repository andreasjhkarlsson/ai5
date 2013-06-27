#pragma once
#include <memory>
#include <string>
#include <vector>

class Variant;

struct NameIdentifier {
	int globalId;
	int localId;
	int staticId;
};
typedef std::shared_ptr<std::vector<char>> shared_binary;
typedef std::shared_ptr<std::wstring> shared_string;
typedef std::shared_ptr<std::vector<Variant*>> shared_var_list;
shared_string create_shared_string(const std::wstring& string);
shared_string create_shared_string(const wchar_t* str,int len);
std::wstring int2string(int num);