#pragma once

#include <string>
#include <memory>

class LuaObject;

class Lua
{
public:
	Lua();
	~Lua();

	std::unique_ptr<LuaObject> ParseObject(std::wstring obj);

private:
	std::unique_ptr<LuaObject> InitializeObject(std::wstring rawObj);
	void RemoveSpecialCharacters(std::wstring& content);
	std::wstring StripObjectLiteralBrackets(std::wstring rawObj);
	bool IsObject(std::wstring data);
};