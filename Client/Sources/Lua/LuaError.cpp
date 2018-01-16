#include "LuaError.hpp"

LuaError::LuaError(const char * message) 
	: runtime_error{ message }
{}
