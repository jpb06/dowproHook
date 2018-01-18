#include "RegistryError.hpp"

RegistryError::RegistryError(const string message, LONG errorCode)
	: runtime_error{ message }
	, errorCode{ errorCode }
{}

LONG RegistryError::ErrorCode() const noexcept
{
	return errorCode;
}