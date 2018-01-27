#pragma once

#include <string>
#include <vector>
#include <Windows.h>

namespace Registry 
{
	unsigned long RegGetDword(HKEY hKey, const std::wstring& subKey, const std::wstring& value);
	std::wstring RegGetString(HKEY hKey, const std::wstring& subKey, const std::wstring& value);
	std::vector<std::wstring> RegGetMultiString(HKEY hKey, const std::wstring& subKey, const std::wstring& value);
}