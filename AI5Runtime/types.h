#pragma once
#include <memory>
#include <string>
#include <vector>

class Variant;

typedef std::shared_ptr<std::wstring> shared_string;

typedef std::shared_ptr<std::vector<Variant*>> shared_var_list;

inline shared_string create_shared_string(const std::wstring& string)
{
	return shared_string(new std::wstring(string));
}


struct NameIdentifier {
	int globalId;
	int localId;
	int staticId;
};