#pragma once
#include "BoredData.h"
#include <string>

class BoredBool: public BoredData
{
private:
	bool data;

public:
	BoredBool() = default;
	BoredBool(bool val);

	std::string ToString();
};

