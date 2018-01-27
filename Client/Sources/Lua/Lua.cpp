#include "Lua.hpp"

#include <algorithm>
#include <vector>
#include ".\LuaObject.hpp"
#include ".\LuaProperty.hpp"
#include ".\LuaElement.hpp"
#include ".\..\Errors\LuaError.hpp"
#include ".\..\Util\StringUtil.hpp"

Lua::Lua()
{}

Lua::~Lua()
{}

std::unique_ptr<LuaObject> Lua::ParseObject(std::wstring obj)
{
	Lua::RemoveSpecialCharacters(obj);

	std::unique_ptr<LuaObject> luaObject = Lua::InitializeObject(obj);

	std::wstring rawContent = Lua::StripObjectLiteralBrackets(luaObject->GetRawValue());
	while (rawContent.size() > 0)
	{
		size_t equalPosition = rawContent.find(L"=");
		size_t commaPosition = rawContent.find(L",");
		size_t bracketPosition = rawContent.find(L"{");

		if (equalPosition == std::string::npos && commaPosition == std::string::npos && bracketPosition == std::string::npos)
			break;

		std::wstring identifier = StringUtil::TrimCopy(rawContent.substr(0, equalPosition));
		std::wstring value;

		if (commaPosition < bracketPosition)
		{
			value = StringUtil::TrimCopy(rawContent.substr(equalPosition + 1, commaPosition - equalPosition - 1));
			rawContent = rawContent.substr(commaPosition + 1);
		}
		else
		{
			size_t endBracketPosition = rawContent.find(L"}");
			value = StringUtil::TrimCopy(rawContent.substr(equalPosition + 1, endBracketPosition - equalPosition));
			rawContent = rawContent.substr(endBracketPosition + 2);
		}

		if(Lua::IsObject(value))
		{
			std::unique_ptr<LuaObject> sub = Lua::ParseObject(identifier + L" = " + value);
			luaObject->AddObj(move(sub));
		}
		else
		{
			LuaElementType type = StringUtil::IsNumber(value) ? LuaElementType::Number : LuaElementType::String;			
			luaObject->AddProp(new LuaProperty(type, identifier, value));
		}
	}

	return luaObject;
}

std::unique_ptr<LuaObject> Lua::InitializeObject(std::wstring rawObj)
{
	size_t equalPosition = rawObj.find(L"=");
	std::wstring left = StringUtil::TrimCopy(rawObj.substr(0, equalPosition));
	std::wstring right = StringUtil::TrimCopy(rawObj.substr(equalPosition + 1));

	if (!Lua::IsObject(right))
		throw LuaError("Element is not an object");

	std::unique_ptr<LuaObject> pt(new LuaObject(left, right));
	return pt;
}

void Lua::RemoveSpecialCharacters(std::wstring & content)
{
	content.erase(remove(content.begin(), content.end(), '\n'), content.end());
	content.erase(remove(content.begin(), content.end(), '\t'), content.end());
}

std::wstring Lua::StripObjectLiteralBrackets(std::wstring rawObj)
{
	return rawObj.substr(1, rawObj.size() - 2);
}

bool Lua::IsObject(std::wstring data)
{
	if (data[0] == L'{' && data[data.size() - 1] == L'}')
		return true;
	else
		return false;
}
