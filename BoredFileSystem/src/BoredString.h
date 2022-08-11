#pragma once
#include "BoredData.h"
class BoredString: public BoredData
{
private:
	std::string data;
public:
	BoredString() = default;
	BoredString(const std::string& str) : data(str) {};
	BoredString(const std::string&& str) : data(str) {};

	// Inherited via BoredData
	virtual std::string ToString() override;
};

