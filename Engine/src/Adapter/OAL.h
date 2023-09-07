#pragma once

#include "Audio.h"
#include <vector>
#include <glm/glm.hpp>
#include <AL/al.h>
#include <AL/alc.h>

namespace Bored {
    namespace Audio {
        namespace OAL {
            class Buffer : public Bored::Audio::Buffer
            {
                public:
                    Buffer() = default;
                    Buffer(const void *data, size_t size, Format format, size_t freq);
                    virtual ~Buffer() override;

                    virtual int GetFreq() override;
                    virtual int GetBits() override;
                    virtual int GetChannels() override;
                    virtual int GetSize() override;
                    virtual std::vector<int> GetData() override;
                    virtual bool IsValid() override;
                    virtual void BufferData(const void *data, size_t size, Format format, size_t freq) override;

                public:
                    ALuint GetId() const { return id; }

                private:
                    ALenum getFormat(Format format);
                    ALuint id;
            };

            class Source : public Bored::Audio::Source
            {
                public:
                    Source();
                    virtual ~Source() override;

                    virtual bool IsValid() override;

                    virtual void SetPosition(const glm::vec3 &) override;
                    virtual void SetVelocity(const glm::vec3 &) override;
                    virtual void SetDirection(const glm::vec3 &) override;

                    virtual void SetPitch(float) override;
                    virtual void SetGain(float) override;
                    virtual void SetMinGain(float) override;
                    virtual void SetMaxGain(float) override;
                    virtual void SetMaxDistance(float) override;
                    virtual void SetRolloffFactor(float) override;
                    virtual void SetConeOuterGain(float) override;
                    virtual void SetConeInnerAngle(float) override;
                    virtual void SetConeOuterAngle(float) override;
                    virtual void SetReferenceDistance(float) override;

                    virtual void SetSourceRelative(unsigned int) override;
                    virtual void SetLooping(unsigned int) override;
                    virtual void SetBuffer(unsigned int) override;
                    virtual void SetSourceState(unsigned int) override;

                    virtual void AddBuffer(std::shared_ptr<Bored::Audio::Buffer>) override;
                    virtual void Play() override;
                    virtual void Stop() override;
                    virtual void Rewind() override;

                private:
                    ALuint id, bufferId;
                    std::shared_ptr<Bored::Audio::Buffer> buffer;
            };

            class Listener : public Bored::Audio::Listener
            {
                public:
                    Listener() {}
                    virtual ~Listener() override;

                    virtual void SetGain(float) override;
                    virtual void SetPosition(const glm::vec3&) override;
                    virtual void SetVelocity(const glm::vec3&) override;
                    virtual void SetUpDir(const glm::vec3&) override;
                    virtual void SetAtDir(const glm::vec3&) override;

                    virtual float GetGain() const override;
                    virtual glm::vec3 GetPosition() const override;
                    virtual glm::vec3 GetVelocity() const override;
                    virtual glm::vec3 GetUpDir() const override;
                    virtual glm::vec3 GetAtDir() const override;
                private:
                    float gain;
                    glm::vec3 position, velocity, upDir, atDir;
            };

            class Context : public Bored::Audio::Context
            {
                public:
                    Context();
                    virtual ~Context() override;

                    virtual std::shared_ptr<Bored::Audio::Source> CreateSource() override;
                private:
                    ALCcontext* context;
                    std::vector<Source> sources;
            };
        }
    }
}
