#include "SocketError.hpp"

SocketError::SocketError(const string message)
	: runtime_error{ message }
{}
