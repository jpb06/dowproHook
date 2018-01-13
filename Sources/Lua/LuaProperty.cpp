#include "LuaProperty.hpp"

LuaProperty::LuaProperty(wstring identifier, wstring value) 
	: LuaElement(identifier, value)
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
