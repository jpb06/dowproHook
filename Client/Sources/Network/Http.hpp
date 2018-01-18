#pragma once

#include <string>
#include <sstream>
#include "HttpHeaders.hpp"
#include "./../Util/StringUtil.hpp"
#include "./../Errors/HeadersError.hpp"

using namespace std;

class Http
{
public:
	static httpHeaders ParseHeaders(string raw);
	
private:
	static void MapProtocolStatus(string line, httpHeaders& headers);
	static void MapAdditionalHeaders(string line, httpHeaders& headers);
};