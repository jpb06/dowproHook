#pragma once

#include <stdexcept>
#include <string>

using namespace std;

class SocketError : public runtime_error
{
public:
	SocketError(const string message);
};