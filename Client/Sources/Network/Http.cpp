#include "Http.hpp"

#include <sstream>
#include ".\HttpHeaders.hpp"
#include ".\..\Util\StringUtil.hpp"
#include ".\..\Errors\HeadersError.hpp"

httpHeaders Http::ParseHeaders(std::string raw)
{
	httpHeaders headers;

	istringstream stream(raw);

	std::string line;
	int pos = 1;
	while (getline(stream, line)) 
	{
		if(pos == 1) Http::MapProtocolStatus(line, headers);
		else Http::MapAdditionalHeaders(line, headers);

		pos++;
	}

	return headers;
}

void Http::MapProtocolStatus(std::string line, httpHeaders& headers)
{
	size_t length = line.size();
	if (!StringUtil::StartsWith(line, "HTTP/1.") || length <= 13)
		throw HeadersError { "Invalid http headers; at \"" + line + "\"" };

	headers.protocol = line.substr(0, 8);

	int statusCode = 0;
	try
	{
		statusCode = stoi(line.substr(9, 3));
	}
	catch (invalid_argument)
	{
		throw HeadersError { "Invalid http headers; at \"" + line + "\"" };
	}

	headers.statusCode = statusCode;
	headers.status = line.substr(13, length - 14);
}


void Http::MapAdditionalHeaders(std::string line, httpHeaders & headers)
{
	if (line == "\r") return;

	size_t colonPos = line.find(':');

	if(colonPos == std::string::npos)
		throw HeadersError { "Invalid http headers; at \"" + line + "\"" };

	std::string type = line.substr(0, colonPos);
	std::string value = line.substr(colonPos + 1, line.size() - colonPos-2);

	if (type == "Content-Type") headers.contentType = value;
	if (type == "Date") headers.date = value;
	if (type == "Connection") headers.connection = value;
	if (type == "Transfer-Encoding") headers.transferEncoding = value;
}

