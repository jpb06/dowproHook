#include "DateUtil.hpp"

#include <time.h>

std::string DateUtil::GetCurrentFormattedTime()
{
	struct tm tm;
	char buffer[18]; 
	time_t rawtime;
	time(&rawtime);
	localtime_s(&tm, &rawtime);
	strftime(buffer, sizeof(buffer), "%m-%d-%Y_%H%M%S", &tm);

	return std::string(buffer, sizeof(buffer));
}
