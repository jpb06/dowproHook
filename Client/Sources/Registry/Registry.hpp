#pragma once

#include <string>
#include <windows.h>
#include <vector>
#include "./../Errors/RegistryError.hpp"

using namespace std;

namespace Registry 
{
	DWORD RegGetDword(HKEY hKey, const wstring& subKey, const wstring& value);
	wstring RegGetString(HKEY hKey, const wstring& subKey, const wstring& value);
	vector<wstring> RegGetMultiString(HKEY hKey, const wstring& subKey, const wstring& value);
}