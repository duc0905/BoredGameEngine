#include "pch.h"
#include "AudioComponent.h"
#include "../IGame.h"

AudioComponent::AudioComponent()
{
	auto& audio = IGame::GetAudio();
	source_ = audio.CreateSource();
	soundBuffer_ = audio.CreateBuffer();
}

AudioComponent::~AudioComponent()
{
}

void AudioComponent::PlayCompSound()
{
	source_->Play();
}

void AudioComponent::SetGain(float gain)
{
	source_->SetGain(gain);
}

void AudioComponent::SetSound(const std::string& file)
{
	if (!soundBuffer_->LoadWav(file))
	{
		LOG_COLOR("Error reading file", COLOR::RED, COLOR::BLACK);
		return;
	}
	source_->BindBuffer(soundBuffer_->GetId());
	source_->SetGain(1.0f);
	source_->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void AudioComponent::SetSound(std::shared_ptr<ISoundBuffer> buffer)
{
	source_->BindBuffer(buffer->GetId());
	source_->SetGain(1.0f);
	source_->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void AudioComponent::SetPosition(const glm::vec3& pos)
{
	source_->SetPosition(pos);
}

const std::string& AudioComponent::GetComponentName()
{
	// // O: insert return statement here
	return "11";
}
