#pragma once
#include "IAudio.h"
#include "OpenALSource.h"
#include "OpenALBuffer.h"
#include <AudioFile.h>
#include <AL/al.h>
#include <AL/alc.h>

class OpenALAudio : public IAudio
{
public:
	OpenALAudio();
	// Inherited via IAudio
	virtual bool SetListenerPosition(const glm::vec3&) override;
	virtual bool SetListenerVelocity(const glm::vec3&) override;
	virtual bool Play(int sndId) override;
	virtual std::shared_ptr<ISource> CreateSource() override;
	virtual std::shared_ptr<ISoundBuffer> CreateBuffer() override;
};