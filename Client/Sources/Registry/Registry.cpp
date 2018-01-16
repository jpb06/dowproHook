#include "Registry.hpp"

namespace Registry
{

	DWORD RegGetDword(HKEY hKey, const wstring& subKey, const wstring& value)
	{
		DWORD data{};
		DWORD dataSize = sizeof(data);

		LONG retCode = RegGetValue(
			hKey,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_DWORD,
			nullptr,
			&data,
			&dataSize
		);

		if (retCode != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read DWORD from registry.", retCode };

		return data;
	}

	wstring RegGetString(HKEY hKey, const wstring& subKey, const wstring& value)
	{
		DWORD dataSize{};
		LONG retCode = RegGetValue(
			hKey,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			nullptr,
			&dataSize
		);

		if (retCode != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read string from registry", retCode };

		wstring data;
		data.resize(dataSize / sizeof(wchar_t));

		retCode = RegGetValue(
			hKey,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_SZ,
			nullptr,
			&data[0],
			&dataSize
		);

		if (retCode != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read string from registry", retCode };

		DWORD stringLengthInWchars = dataSize / sizeof(wchar_t);

		stringLengthInWchars--; // Exclude the NUL written by the Win32 API
		data.resize(stringLengthInWchars);

		return data;
	}

	vector<wstring> RegGetMultiString(HKEY hKey, const wstring& subKey, const wstring& value)
	{
		DWORD dataSize{};
		LONG retCode = RegGetValue(
			hKey,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_MULTI_SZ,
			nullptr,
			nullptr,
			&dataSize
		);

		if (retCode != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read multi-string from registry", retCode };

		vector<wchar_t> data;
		data.resize(dataSize / sizeof(wchar_t));

		retCode = RegGetValue(
			hKey,
			subKey.c_str(),
			value.c_str(),
			RRF_RT_REG_MULTI_SZ,
			nullptr,
			&data[0],
			&dataSize
		);

		if (retCode != ERROR_SUCCESS)
			throw RegistryError{ "Cannot read multi-string from registry", retCode };

		data.resize(dataSize / sizeof(wchar_t));

		// Parse the double-NUL-terminated string into a vector<wstring>
		vector<wstring> result;
		const wchar_t* currStringPtr = &data[0];
		while (*currStringPtr != L'\0')
		{
			// Current string is NUL-terminated, so get its length with wcslen
			const size_t currStringLength = wcslen(currStringPtr);
			// Add current string to result vector
			result.push_back(wstring{ currStringPtr, currStringLength });
			// Move to the next string
			currStringPtr += currStringLength + 1;
		}

		return result;
	}
}