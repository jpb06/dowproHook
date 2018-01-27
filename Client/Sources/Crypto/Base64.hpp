#pragma once

#include <string>

namespace Crypto 
{
	std::string Base64Encode(unsigned char const*, unsigned int len);
	std::string Base64Decode(std::string const& s);
}