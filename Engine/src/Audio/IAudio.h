#pragma once
#include "../pch.h"
#include "../System.h"
#include "ISoundBuffer.h"
#include "ISource.h"

class IAudio : public System
{
private:
	static std::shared_ptr<IAudio> defaultAudio_;
public:
	static std::shared_ptr<IAudio> GetDefault() { return defaultAudio_; };
	virtual bool Play(int sndId) = 0;
	virtual bool SetListenerPosition(const glm::vec3&) = 0;
	virtual bool SetListenerVelocity(const glm::vec3&) = 0;
	virtual std::shared_ptr<ISource> CreateSource() = 0;
	virtual std::shared_ptr<ISoundBuffer> CreateBuffer() = 0;
};

class NullAudio : public IAudio
{
	// Inherited via IAudio
	virtual bool Play(int sndId) override;
	virtual bool SetListenerPosition(const glm::vec3&) override;
	virtual bool SetListenerVelocity(const glm::vec3&) override;
	virtual std::shared_ptr<ISource> CreateSource() override;
	virtual std::shared_ptr<ISoundBuffer> CreateBuffer() override;
};