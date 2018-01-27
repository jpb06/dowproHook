#include "RegistryError.hpp"

#include <windows.h>

RegistryError::RegistryError(const string message, long errorCode)
	: runtime_error{ message }
	, errorCode{ errorCode }
{}

long RegistryError::ErrorCode() const noexcept
{
	return errorCode;
}