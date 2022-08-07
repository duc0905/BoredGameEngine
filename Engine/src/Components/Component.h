#pragma once
#include <string>

class Component
{
public:
	virtual const std::string& GetComponentName() = 0;
	virtual void Update(double dt) {};
};