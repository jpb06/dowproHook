#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class ApplicationError : public runtime_error
{
public:
	ApplicationError(const string message);
};