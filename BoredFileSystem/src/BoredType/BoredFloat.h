#pragma once
#include"BoredData.h"

class BoredFloat: public BoredData
{
private:
	float data;
public:
	BoredFloat() = default;
	BoredFloat(const float& f) : data(f) {};
	BoredFloat(const float&& f) : data(f) {};

	// Inherited via BoredData
	virtual std::string ToString() override;
};

