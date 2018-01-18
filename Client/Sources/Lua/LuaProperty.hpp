#pragma once

#include <string>
#include "LuaElement.hpp"
#include "./../Errors/LuaError.hpp"

using namespace std;

class LuaProperty : public LuaElement
{
public:
	LuaProperty(LuaElementType type, wstring identifier, wstring value);
	~LuaProperty();

	int AsInt () const;
	wstring AsString () const;

	wstring ToJson() const;
};