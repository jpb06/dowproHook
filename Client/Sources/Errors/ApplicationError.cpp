#include "ApplicationError.hpp"

ApplicationError::ApplicationError(const string message)
	: runtime_error{ message }
{}