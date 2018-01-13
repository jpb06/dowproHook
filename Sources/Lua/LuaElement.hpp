#pragma once

#include <string>

using namespace std;

class LuaElement
{
public:
	LuaElement(wstring identifier, wstring rawValue);
	~LuaElement();

	wstring GetIdentifier() const;
	wstring GetRawValue() const;

private:
	wstring identifier;
	wstring rawValue;
};