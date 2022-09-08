#pragma once
#include "../pch.h"
class ISource
{
public:
	virtual bool BindBuffer(unsigned int) = 0;
	virtual bool Play() = 0;
	virtual bool SetGain(float) = 0;
	virtual bool SetVelocity(const glm::vec3&) = 0;
	virtual bool SetPosition(const glm::vec3&) = 0;
};

