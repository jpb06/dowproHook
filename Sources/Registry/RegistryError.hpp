#pragma once

#include <stdexcept>
#include <windows.h>

using namespace std;

class RegistryError : public runtime_error
{
	public:
		RegistryError(const char* message, LONG errorCode);
		LONG ErrorCode() const noexcept;
	private:
		LONG m_errorCode;
};