#include "Lua.hpp"

Lua::Lua()
{}

Lua::~Lua()
{}

unique_ptr<LuaObject> Lua::ParseObject(wstring obj)
{
	Lua::RemoveSpecialCharacters(obj);

	unique_ptr<LuaObject> luaObject = Lua::InitializeObject(obj);

	wstring rawContent = Lua::StripObjectLiteralBrackets(luaObject->GetRawValue());
	while (rawContent.size() > 0)
	{
		size_t equalPosition = rawContent.find(L"=");
		size_t commaPosition = rawContent.find(L",");
		size_t bracketPosition = rawContent.find(L"{");

		if (equalPosition == string::npos && commaPosition == string::npos && bracketPosition == string::npos)
			break;

		wstring identifier = StringUtil::TrimCopy(rawContent.substr(0, equalPosition));
		wstring value;

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
			unique_ptr<LuaObject> sub = Lua::ParseObject(identifier + L" = " + value);
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

unique_ptr<LuaObject> Lua::InitializeObject(wstring rawObj)
{
	size_t equalPosition = rawObj.find(L"=");
	wstring left = StringUtil::TrimCopy(rawObj.substr(0, equalPosition));
	wstring right = StringUtil::TrimCopy(rawObj.substr(equalPosition + 1));

	if (!Lua::IsObject(right))
		throw LuaError("Element is not an object");

	unique_ptr<LuaObject> pt(new LuaObject(left, right));
	return pt;
}

void Lua::RemoveSpecialCharacters(wstring & content)
{
	content.erase(remove(content.begin(), content.end(), '\n'), content.end());
	content.erase(remove(content.begin(), content.end(), '\t'), content.end());
}

wstring Lua::StripObjectLiteralBrackets(wstring rawObj)
{
	return rawObj.substr(1, rawObj.size() - 2);
}

bool Lua::IsObject(wstring data)
{
	if (data[0] == L'{' && data[data.size() - 1] == L'}')
		return true;
	else
		return false;
}
