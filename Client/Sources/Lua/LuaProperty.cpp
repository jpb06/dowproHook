#include "LuaProperty.hpp"

#include ".\LuaElement.hpp"
#include ".\..\Errors\LuaError.hpp"

LuaProperty::LuaProperty(LuaElementType type, std::wstring identifier, std::wstring value)
	: LuaElement(type, identifier, value)
{}

LuaProperty::~LuaProperty()
{}

int LuaProperty::AsInt() const
{
	int val = 0;
	try
	{
		val = stoi(this->GetRawValue());
		return val;
	}
	catch (const invalid_argument&)
	{
		throw LuaError { "Conversion to int failed" };
	}
}

std::wstring LuaProperty::AsString() const
{
	std::wstring val = this->GetRawValue();
	size_t length = val.size();

	if(val[0] != '"' || val[length-1] != '"')
		throw LuaError { "Value is not a string" };
	
	return val.substr(1, length - 2);
}

std::wstring LuaProperty::ToJson() const
{
	std::wstring jsonValue;
	std::wstring rawValue = this->GetRawValue();

	size_t length = rawValue.size();
	if (rawValue[0] == '"' && rawValue[length - 1] == '"')
		jsonValue = rawValue;
	else
		jsonValue = L"\"" + rawValue + L"\"";

	return L"\"" + this->GetIdentifier() + L"\":" + jsonValue;
}
