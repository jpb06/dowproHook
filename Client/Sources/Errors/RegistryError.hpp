#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class RegistryError : public runtime_error
{
	public:
		RegistryError(const string message, long errorCode);
		long ErrorCode() const noexcept;
	private:
		long errorCode;
};