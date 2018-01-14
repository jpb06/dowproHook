#include "LuaElement.hpp"

LuaElement::LuaElement(LuaElementType type, wstring identifier, wstring rawValue)
{
	this->type = type;
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

LuaElementType LuaElement::GetLuaType() const
{
	return this->type;
}
