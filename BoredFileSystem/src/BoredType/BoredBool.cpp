#include "BoredBool.h"

BoredBool::BoredBool(bool val)
{
	data = val;
}

std::string BoredBool::ToString()
{
	return data ? "True" : "False";
}

