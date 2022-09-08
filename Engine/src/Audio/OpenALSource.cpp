#include "pch.h"
#include "OpenALSource.h"

OpenALSource::OpenALSource()
{
    alGenSources(1, &id_);
}

bool OpenALSource::BindBuffer(ALuint bufferId)
{
    bufferId_ = bufferId;
    alSourcei(id_, AL_BUFFER, bufferId_);
    return true;
}

bool OpenALSource::Play()
{
    alSourcePlay(id_);
    return true;
}

bool OpenALSource::SetGain(float gain)
{
    alSourcef(id_, AL_GAIN, gain);
    return true;
}

bool OpenALSource::SetVelocity(const glm::vec3& v)
{
    alSource3f(id_, AL_VELOCITY, v.x, v.y, v.z);
    return true;
}

bool OpenALSource::SetPosition(const glm::vec3& p)
{
    alSource3f(id_, AL_POSITION, p.x, p.y, p.z);
    return true;
}
