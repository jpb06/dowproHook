#pragma once

#include <string>
#include <windows.h>
#include <vector>
#include "RegistryError.h"

using namespace std;

class Registry
{
	public:
		Registry();
		~Registry();

		DWORD RegGetDword(HKEY hKey, const wstring& subKey, const wstring& value);
		wstring RegGetString(HKEY hKey, const wstring& subKey, const wstring& value);
		vector<wstring> RegGetMultiString(HKEY hKey, const wstring& subKey, const wstring& value);
};