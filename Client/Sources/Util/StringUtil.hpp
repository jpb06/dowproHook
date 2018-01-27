#pragma once

#include <string>

namespace StringUtil
{
	void TrimLeft(std::wstring &s); // trim from start (in place)
	void TrimRight(std::wstring &s); // trim from end (in place)
	void Trim(std::wstring &s); // trim from both ends (in place)
	std::wstring TrimLeftCopy(std::wstring s); // trim from start (copying)
	std::wstring TrimRightCopy(std::wstring s); // trim from end (copying)
	std::wstring TrimCopy(std::wstring s); // trim from both ends (copying)
	std::string ConvertToNarrow(std::wstring ws);
	std::wstring ConvertToWide(std::string s);
	void RemoveIllegalCharacters(std::wstring* ws);
	bool IsNumber(const std::string& s);
	bool IsNumber(const std::wstring& s);
	bool StartsWith(const std::string& s, const std::string& prefix);
}