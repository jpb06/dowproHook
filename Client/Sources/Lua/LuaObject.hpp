#pragma once

#include <vector>
#include <string>
#include <memory>
#include ".\LuaElement.hpp"

class LuaProperty;

class LuaObject : public LuaElement
{
public:
	LuaObject(std::wstring identifier, std::wstring rawContent);
	~LuaObject();

	template<typename T>
	T const* Get(int index) const;
	template<typename T>
	T const* Get(const std::wstring identifier) const;

	void AddObj(std::unique_ptr<LuaObject> pt);
	void AddProp(LuaProperty* pt);

	std::wstring ToJson() const;
private:
	std::vector<std::unique_ptr<LuaElement>> properties;
};

template<typename T>
inline T const* LuaObject::Get(int index) const
{
	static_assert(is_base_of<LuaElement, T>::value, "T should inherit from LuaElement");
	
	LuaElement* el = this->properties[index].get();
	return static_cast<T*>(el);
}

template<typename T>
inline T const* LuaObject::Get(const std::wstring identifier) const
{
	static_assert(is_base_of<LuaElement, T>::value, "T should inherit from LuaElement");

	std::vector<std::unique_ptr<LuaElement>>::const_iterator results = find_if(
		this->properties.begin(),
		this->properties.end(),
		[&](std::unique_ptr<LuaElement> const& p) { return p->GetIdentifier() == identifier; }
	);

	if (results == this->properties.end()) {
		return nullptr;
	}

	LuaElement* el = results->get();
	return static_cast<T*>(el);
}