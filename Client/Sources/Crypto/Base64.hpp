#pragma once

#include <string>

using namespace std;

namespace Crypto 
{
	string Base64Encode(unsigned char const*, unsigned int len);
	string Base64Decode(string const& s);
}