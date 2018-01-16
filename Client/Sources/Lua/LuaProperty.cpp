#include "LuaProperty.hpp"

LuaProperty::LuaProperty(LuaElementType type, wstring identifier, wstring value)
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

wstring LuaProperty::AsString() const
{
	wstring val = this->GetRawValue();
	size_t length = val.size();

	if(val[0] != '"' || val[length-1] != '"')
		throw LuaError { "Value is not a string" };
	
	return val.substr(1, length - 2);
}

wstring LuaProperty::ToJson() const
{
	wstring jsonValue;
	wstring rawValue = this->GetRawValue();

	size_t length = rawValue.size();
	if (rawValue[0] == '"' && rawValue[length - 1] == '"')
		jsonValue = rawValue;
	else
		jsonValue = L"\"" + rawValue + L"\"";

	return L"\"" + this->GetIdentifier() + L"\":" + jsonValue;
}
