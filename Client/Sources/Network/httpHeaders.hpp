#pragma once

#include <string>

struct httpHeaders
{
	std::string protocol;
	int statusCode;
	std::string status;

	std::string contentType;
	std::string date;
	std::string connection;
	std::string transferEncoding;
};