#pragma once
#include "../pch.h"

class Component
{
public:
	virtual const std::string& GetComponentName() = 0;
	virtual void Update(double dt) {};
};