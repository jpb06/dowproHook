#pragma once

#include <string>

using namespace std;

struct httpHeaders
{
	string protocol;
	int statusCode;
	string status;

	string contentType;
	string date;
	string connection;
	string transferEncoding;
};