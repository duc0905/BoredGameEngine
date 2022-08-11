#pragma once
#include "BoredData.h"

class BoredInt: public BoredData
{
private:
	int data = 0;
public:
	BoredInt() = default;
	BoredInt(const int& d) : data(d) {};
	BoredInt(const int&& d) : data(d) {};


	// Inherited via BoredData
	virtual std::string ToString() override;
};

