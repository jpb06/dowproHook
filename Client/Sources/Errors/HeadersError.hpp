#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class HeadersError : public runtime_error
{
public:
	HeadersError(const string message);
};