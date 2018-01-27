#pragma once

#include <string>
#include "LuaElement.hpp"

class LuaProperty : public LuaElement
{
public:
	LuaProperty(LuaElementType type, std::wstring identifier, std::wstring value);
	~LuaProperty();

	int AsInt () const;
	std::wstring AsString () const;

	std::wstring ToJson() const;
};