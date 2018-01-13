#pragma once

#include <string>
#include "LuaElement.hpp"
#include "LuaError.hpp"

using namespace std;

class LuaProperty : public LuaElement
{
public:
	LuaProperty(wstring identifier, wstring value);
	~LuaProperty();

	int AsInt () const;
	wstring AsString () const;
};