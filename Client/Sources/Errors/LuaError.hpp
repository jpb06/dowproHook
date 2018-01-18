#pragma once

#include <stdexcept>

using namespace std;

class LuaError : public runtime_error
{
public:
	LuaError(const string message);
};