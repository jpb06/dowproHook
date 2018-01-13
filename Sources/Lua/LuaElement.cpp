#include "LuaElement.hpp"

LuaElement::LuaElement(wstring identifier, wstring rawValue)
{
	this->identifier = identifier;
	this->rawValue = rawValue;
}

LuaElement::~LuaElement()
{}

wstring LuaElement::GetIdentifier() const
{
	return this->identifier;
}

wstring LuaElement::GetRawValue() const
{
	return this->rawValue;
}