#pragma once

#include <string>

enum LuaElementType { String, Number, Object };

using namespace std;

class LuaElement
{
public:
	LuaElement(LuaElementType type, wstring identifier, wstring rawValue);
	~LuaElement();

	wstring GetIdentifier() const;
	wstring GetRawValue() const;
	LuaElementType GetLuaType() const;

private:
	wstring identifier;
	wstring rawValue;
	LuaElementType type;
};