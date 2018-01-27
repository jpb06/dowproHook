#include "LuaObject.hpp"

#include ".\LuaProperty.hpp"
#include ".\..\Errors\LuaError.hpp"

LuaObject::LuaObject(std::wstring identifier, std::wstring rawContent)
	: LuaElement(LuaElementType::Object, identifier, rawContent)
{}

LuaObject::~LuaObject()
{}

void LuaObject::AddObj(std::unique_ptr<LuaObject> pt)
{
	this->properties.push_back(move(pt));
}

void LuaObject::AddProp(LuaProperty* pt)
{
	this->properties.emplace_back(pt);
}

std::wstring LuaObject::ToJson() const
{
	std::wstring content = L"{";

	unsigned int size = this->properties.size();
	for (unsigned int i = 0; i < this->properties.size(); i++)
	{
		const LuaElement* element = this->Get<LuaElement>(i);
		switch (element->GetLuaType())
		{
			case LuaElementType::Object:
				content += L"\"" + element->GetIdentifier() + L"\":" + this->Get<LuaObject>(i)->ToJson();
				break;
			case LuaElementType::Number:
			case LuaElementType::String:
				content += this->Get<LuaProperty>(i)->ToJson();
				break;
		}

		if (i < size - 1) content += L",";
	}

	content += L"}";

	return content;
}