#include "pch.h"
#include "IAudio.h"

std::shared_ptr<IAudio> IAudio::defaultAudio_ = std::make_shared<NullAudio>();
bool NullAudio::Play(int sndId)
{
    return true;
}

bool NullAudio::SetListenerPosition(const glm::vec3&)
{
    return true;
}

bool NullAudio::SetListenerVelocity(const glm::vec3&)
{
    return true;
}

std::shared_ptr<ISource> NullAudio::CreateSource()
{
    return nullptr;
}

std::shared_ptr<ISoundBuffer> NullAudio::CreateBuffer()
{
    return nullptr;
}
