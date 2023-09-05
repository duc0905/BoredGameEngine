#pragma once
#include <string>
#include <memory>
#include <vector>
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
            virtual ~Buffer(){};

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

        class Source
        {
        public:
            virtual ~Source(){};
            virtual bool IsValid() = 0;

            virtual void SetPosition(const glm::vec3 &) = 0;
            virtual void SetVelocity(const glm::vec3 &) = 0;
            virtual void SetDirection(const glm::vec3 &) = 0;

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

            virtual void AddBuffer(std::shared_ptr<Buffer>) = 0;
            virtual void Play() = 0;
            virtual void Stop() = 0;
            virtual void Rewind() = 0;

            // TODO Add getters
        };

        class Listener
        {
        public:
            virtual ~Listener() {};

            virtual void SetGain(float) = 0;
            virtual void SetPosition(const glm::vec3&) = 0;
            virtual void SetVelocity(const glm::vec3&) = 0;
            virtual void SetUpDir(const glm::vec3&) = 0;
            virtual void SetAtDir(const glm::vec3&) = 0;

            virtual float GetGain() const = 0;
            virtual glm::vec3 GetPosition() const = 0;
            virtual glm::vec3 GetVelocity() const = 0;
            virtual glm::vec3 GetUpDir() const = 0;
            virtual glm::vec3 GetAtDir() const = 0;
        };

        class Context
        {
        public:
            Context(std::shared_ptr<Listener> l) : listener(l) {};
            virtual ~Context() {};

            virtual std::shared_ptr<Source> CreateSource() = 0;
            std::shared_ptr<Listener> GetListener() { return listener; };
        protected:
            std::shared_ptr<Listener> listener;
        };
    }
}
