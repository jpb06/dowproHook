#include "LuaObject.hpp"

LuaObject::LuaObject(wstring identifier, wstring rawContent) 
	: LuaElement(identifier, rawContent)
{}

LuaObject::~LuaObject()
{}

void LuaObject::AddObj(unique_ptr<LuaObject> pt)
{
	this->properties.push_back(move(pt));
}

void LuaObject::AddProp(LuaProperty* pt)
{
	this->properties.emplace_back(pt);
}