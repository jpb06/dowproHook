#include "LuaError.hpp"

LuaError::LuaError(const string message)
	: runtime_error{ message }
{}
