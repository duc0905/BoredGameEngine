#pragma once
#include <string>
#include <memory>
#include <vector>
#include <AL/alc.h>
#include <AL/al.h>
#include <glm/glm.hpp>

namespace Bored
{
    namespace Audio
    {
        enum Format
        {
            MONO8 = 0x1100,   // 8-bit mono
            MONO16 = 0x1101,  // 16-bit mono
            STEREO8 = 0x1102, // 8-bit stereo
            STEREO16 = 0x1103 // 16-bit stereo
        };

        class Buffer
        {
        public:
            Buffer() = default;
            Buffer(const void *data, size_t size, Format format, size_t freq);
            virtual ~Buffer() {};

            virtual bool IsValid() = 0;
            virtual void BufferData(const void *data, size_t size, Format format, size_t freq) = 0;

            virtual int GetFreq() = 0;
            virtual int GetBits() = 0;
            virtual int GetChannels() = 0;
            virtual int GetSize() = 0;
            virtual std::vector<int> GetData() = 0;
        private:
            size_t size, freq;
            Format format;
        };

        class OALBuffer : public Buffer
        {
        public:
            OALBuffer() = default;
            virtual ~OALBuffer() override;
            OALBuffer(const void *data, size_t size, Format format, size_t freq);

            virtual int GetFreq() override;
            virtual int GetBits() override;
            virtual int GetChannels() override;
            virtual int GetSize() override;
            virtual std::vector<int> GetData() override;
            virtual bool IsValid() override;
            virtual void BufferData(const void *data, size_t size, Format format, size_t freq) override;
        private:
            ALenum getFormat(Format format);
            ALuint id;
        };

        class Source
        {
        public:
            virtual ~Source() {};
            virtual bool IsValid() = 0;

            virtual void SetPosition(const glm::vec3&) = 0;
            virtual void SetVelocit(const glm::vec3&) = 0;
            virtual void SetDirection(const glm::vec3&) = 0;

            virtual void SetPitch(float) = 0;
            virtual void SetGain(float) = 0;
            virtual void SetMinGain(float) = 0;
            virtual void SetMaxGain(float) = 0;
            virtual void SetMaxDistance(float) = 0;
            virtual void SetRolloffFactor(float) = 0;
            virtual void SetConeOuterGain(float) = 0;
            virtual void SetConeInnerAngle(float) = 0;
            virtual void SetConeOuterAngle(float) = 0;
            virtual void SetReferenceDistance(float) = 0;

            virtual void SetSourceRelative(unsigned int) = 0;
            virtual void SetLooping(unsigned int) = 0;
            virtual void SetBuffer(unsigned int) = 0;
            virtual void SetSourceState(unsigned int) = 0;
        };

        class OALSource : public Source {
        public:
            OALSource();
            virtual ~OALSource() override;

            virtual bool IsValid() override;

            virtual void SetPosition(const glm::vec3&) override;
            virtual void SetVelocit(const glm::vec3&) override;
            virtual void SetDirection(const glm::vec3&) override;

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
        private:
            ALuint source;
            // @PTM thich de j vao day thi de
        };

        class Listener
        {
        public:
            virtual ~Listener() {};
        };

        class OALListener : public Listener {
        public:
            virtual ~OALListener() override;
        };

        class Context
        {
        public:
            virtual ~Context() {};
        };
    }
}