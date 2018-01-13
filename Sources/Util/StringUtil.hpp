#pragma once

#include <algorithm> 
#include <cctype>
#include <locale>
#include <codecvt>

using namespace std;

namespace StringUtil
{
	void TrimLeft(wstring &s); // trim from start (in place)
	void TrimRight(wstring &s); // trim from end (in place)
	void Trim(wstring &s); // trim from both ends (in place)
	wstring TrimLeftCopy(wstring s); // trim from start (copying)
	wstring TrimRightCopy(wstring s); // trim from end (copying)
	wstring TrimCopy(wstring s); // trim from both ends (copying)
	string ConvertToNarrow(wstring ws);
	wstring ConvertToWide(string s);
	void RemoveIllegalCharacters(wstring* ws);
}