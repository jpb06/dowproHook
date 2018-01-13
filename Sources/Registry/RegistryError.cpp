#include "RegistryError.hpp"

RegistryError::RegistryError(const char* message, LONG errorCode)
	: runtime_error{ message }
	, m_errorCode{ errorCode }
{}

LONG RegistryError::ErrorCode() const noexcept
{
	return m_errorCode;
}