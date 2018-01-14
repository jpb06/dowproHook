#pragma once

#include <vector>
#include <string>
#include <memory>
#include "LuaElement.hpp"
#include "LuaProperty.hpp"
#include "LuaError.hpp"

using namespace std;

class LuaObject : public LuaElement
{
public:
	LuaObject(wstring identifier, wstring rawContent);
	~LuaObject();

	template<typename T>
	T const* Get(int index) const;
	template<typename T>
	T const* Get(const wstring identifier) const;

	void AddObj(unique_ptr<LuaObject> pt);
	void AddProp(LuaProperty* pt);

	wstring ToJson() const;
private:
	vector<unique_ptr<LuaElement>> properties;
};

template<typename T>
inline T const* LuaObject::Get(int index) const
{
	static_assert(is_base_of<LuaElement, T>::value, "T should inherit from LuaElement");
	
	LuaElement* el = this->properties[index].get();
	return static_cast<T*>(el);
}

template<typename T>
inline T const* LuaObject::Get(const wstring identifier) const
{
	static_assert(is_base_of<LuaElement, T>::value, "T should inherit from LuaElement");

	vector<unique_ptr<LuaElement>>::const_iterator results = find_if(
		this->properties.begin(),
		this->properties.end(),
		[&](unique_ptr<LuaElement> const& p) { return p->GetIdentifier() == identifier; }
	);

	if (results == this->properties.end()) {
		return nullptr;
	}

	LuaElement* el = results->get();
	return static_cast<T*>(el);
}