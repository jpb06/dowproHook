#pragma once

#include <string>

enum LuaElementType { String, Number, Object };

class LuaElement
{
public:
	LuaElement(LuaElementType type, std::wstring identifier, std::wstring rawValue);
	~LuaElement();

	std::wstring GetIdentifier() const;
	std::wstring GetRawValue() const;
	LuaElementType GetLuaType() const;

private:
	std::wstring identifier;
	std::wstring rawValue;
	LuaElementType type;
};