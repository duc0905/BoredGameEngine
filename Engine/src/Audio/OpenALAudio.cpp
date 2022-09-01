#include "pch.h"
#include "OpenALAudio.h"

OpenALAudio::OpenALAudio()
{
	ALCdevice* Device = alcOpenDevice(NULL);
	if (Device) {
		ALCcontext* Context = alcCreateContext(Device, NULL);
		alcMakeContextCurrent(Context);
	}

	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		LOG_COLOR("alGenBuffers :" + error, COLOR::RED, COLOR::BLACK);
	}
	LOG_COLOR("Initialized OpenAL", COLOR::BLUE, COLOR::BLACK);
}

bool OpenALAudio::Play(int sourceId)
{
	alSourcePlay(sourceId);
	return true;
}

std::shared_ptr<ISource> OpenALAudio::CreateSource()
{
	return std::make_shared<OpenALSource>();
}

std::shared_ptr<ISoundBuffer> OpenALAudio::CreateBuffer()
{
	return std::make_shared<OpenALBuffer>();
}

bool OpenALAudio::SetListenerPosition(const glm::vec3& p)
{
	alListener3f(AL_POSITION, p.x, p.y, p.z);
	return true;
}

bool OpenALAudio::SetListenerVelocity(const glm::vec3& v)
{
	alListener3f(AL_VELOCITY, v.x, v.y, v.z);
	return true;
}
