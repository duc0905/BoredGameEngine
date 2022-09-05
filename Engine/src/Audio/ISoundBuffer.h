#pragma once
#include  "../pch.h"
class ISoundBuffer
{
public:
	virtual const unsigned int GetId() const = 0;
	virtual bool LoadWav(const std::string& file) = 0;
	virtual bool CleanUp() = 0;
};

