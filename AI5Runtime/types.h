#pragma once
#include <memory>
#include <string>

typedef std::shared_ptr<std::wstring> shared_string;

inline shared_string create_shared_string(const std::wstring& string)
{
	return shared_string(new std::wstring(string));
}