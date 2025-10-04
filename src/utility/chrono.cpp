#include "../../include/tr/utility/chrono.hpp"

std::tm tr::localtime(std::time_t time)
{
#ifdef _MSC_VER
	return *std::localtime(&time);
#else
	std::tm temp;
	return *localtime_r(&time, &temp);
#endif
}