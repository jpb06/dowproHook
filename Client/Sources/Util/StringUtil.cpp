#include "StringUtil.hpp"

namespace StringUtil
{
	void TrimLeft(wstring &s) 
	{
		s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
			return !isspace(ch);
		}));
	}

	void TrimRight(wstring &s) 
	{
		s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
			return !isspace(ch);
		}).base(), s.end());
	}

	void Trim(wstring &s) 
	{
		TrimLeft(s);
		TrimRight(s);
	}

	wstring TrimLeftCopy(wstring s) 
	{
		TrimLeft(s);
		return s;
	}

	wstring TrimRightCopy(wstring s) 
	{
		TrimRight(s);
		return s;
	}

	wstring TrimCopy(wstring s) 
	{
		Trim(s);
		return s;
	}

	string ConvertToNarrow(wstring ws)
	{
		//setup converter
		using convert_type = codecvt_utf8<wchar_t>;
		wstring_convert<convert_type, wchar_t> converter;

		string narrow = converter.to_bytes(ws);
		return narrow;
	}

	wstring ConvertToWide(string s)
	{
		//setup converter
		using convert_type = codecvt_utf8<wchar_t>;
		wstring_convert<convert_type, wchar_t> converter;

		wstring wide = converter.from_bytes(s);
		return wide;
	}

	void RemoveIllegalCharacters(wstring* ws)
	{
		wstring illegalChars = L"<>:\"/?*|\\";

		wstring::iterator it;

		for (it = ws->begin(); it < ws->end(); ++it)
		{
			bool found = illegalChars.find(*it) != string::npos;
			if (found)
				*it = L'_';
		}
	}

	bool IsNumber(const string& s)
	{
		return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
	}

	bool IsNumber(const wstring& s)
	{
		return IsNumber(ConvertToNarrow(s));
	}

	bool StartsWith(const string& s, const string& prefix) 
	{
		return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
	}
}