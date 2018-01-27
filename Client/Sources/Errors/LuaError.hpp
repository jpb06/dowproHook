#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class LuaError : public runtime_error
{
public:
	LuaError(const string message);
};