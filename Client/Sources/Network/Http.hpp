#pragma once

#include <string>
#include "httpHeaders.hpp"

class Http
{
public:
	static httpHeaders ParseHeaders(std::string raw);
	
private:
	static void MapProtocolStatus(std::string line, httpHeaders& headers);
	static void MapAdditionalHeaders(std::string line, httpHeaders& headers);
};