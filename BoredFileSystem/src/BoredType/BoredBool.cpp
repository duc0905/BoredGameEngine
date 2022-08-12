#include "BoredBool.h"
#include <string>

BoredBool::BoredBool(bool val)
{
	data = val;
}

std::string BoredBool::ToString()
{
	std::string ans = data ? "true" : "false";
	return ans;
}

