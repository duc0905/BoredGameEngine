#pragma once
#include <AL/al.h>
#include "ISoundBuffer.h"
class OpenALBuffer : public ISoundBuffer
{
private:
	ALuint id_ = 0;

public:
	OpenALBuffer();
	// Inherited via ISoundBuffer
	virtual const unsigned int GetId() const override;
	virtual bool LoadWav(const std::string& file) override;
	virtual bool CleanUp() override;
};

