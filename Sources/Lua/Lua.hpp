#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include "LuaObject.hpp"
#include "LuaProperty.hpp"
#include "LuaElement.hpp"
#include "./../Util/StringUtil.hpp"

using namespace std;

class Lua
{
public:
	Lua();
	~Lua();

	unique_ptr<LuaObject> ParseObject(wstring obj);

private:
	unique_ptr<LuaObject> InitializeObject(wstring rawObj);
	void RemoveSpecialCharacters(wstring& content);
	wstring StripObjectLiteralBrackets(wstring rawObj);
	bool IsObject(wstring data);
};