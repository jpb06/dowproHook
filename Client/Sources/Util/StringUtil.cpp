#include "StringUtil.hpp"

#include <algorithm> 
#include <cctype>
#include <locale>
#include <codecvt>

namespace StringUtil
{
	void TrimLeft(std::wstring &s)
	{
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
			return !isspace(ch);
		}));
	}

	void TrimRight(std::wstring &s)
	{
		s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
			return !isspace(ch);
		}).base(), s.end());
	}

	void Trim(std::wstring &s)
	{
		TrimLeft(s);
		TrimRight(s);
	}

	std::wstring TrimLeftCopy(std::wstring s)
	{
		TrimLeft(s);
		return s;
	}

	std::wstring TrimRightCopy(std::wstring s)
	{
		TrimRight(s);
		return s;
	}

	std::wstring TrimCopy(std::wstring s)
	{
		Trim(s);
		return s;
	}

	std::string ConvertToNarrow(std::wstring ws)
	{
		//setup converter
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		std::string narrow = converter.to_bytes(ws);
		return narrow;
	}

	std::wstring ConvertToWide(std::string s)
	{
		//setup converter
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		std::wstring wide = converter.from_bytes(s);
		return wide;
	}

	void RemoveIllegalCharacters(std::wstring* ws)
	{
		std::wstring illegalChars = L"<>:\"/?*|\\";

		std::wstring::iterator it;

		for (it = ws->begin(); it < ws->end(); ++it)
		{
			bool found = illegalChars.find(*it) != std::string::npos;
			if (found)
				*it = L'_';
		}
	}

	bool IsNumber(const std::string& s)
	{
		return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
	}

	bool IsNumber(const std::wstring& s)
	{
		return IsNumber(ConvertToNarrow(s));
	}

	bool StartsWith(const std::string& s, const std::string& prefix)
	{
		return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
	}
}