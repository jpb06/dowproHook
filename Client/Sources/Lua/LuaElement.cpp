#include "LuaElement.hpp"

LuaElement::LuaElement(LuaElementType type, std::wstring identifier, std::wstring rawValue)
{
	this->type = type;
	this->identifier = identifier;
	this->rawValue = rawValue;
}

LuaElement::~LuaElement()
{}

std::wstring LuaElement::GetIdentifier() const
{
	return this->identifier;
}

std::wstring LuaElement::GetRawValue() const
{
	return this->rawValue;
}

LuaElementType LuaElement::GetLuaType() const
{
	return this->type;
}
