#pragma once
#include <AL/al.h>
#include "ISource.h"
class OpenALSource : public ISource
{
private:
	ALuint id_ = 0;
	ALuint bufferId_ = 0;
public:
	OpenALSource();
	// Inherited via ISource
	virtual bool BindBuffer(ALuint) override;
	virtual bool Play() override;
	virtual bool SetGain(float) override;
	virtual bool SetVelocity(const glm::vec3&) override;
	virtual bool SetPosition(const glm::vec3&) override;
};

