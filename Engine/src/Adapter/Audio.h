#pragma once
#include <memory>
#include <string>
#include <AL/alc.h>
#include <AL/al.h>

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
            ~Buffer() = default;
            Buffer(const void *data, size_t size, Format format, size_t freq);

            virtual void BufferData(const void* data, size_t size, Format format, size_t freq) = 0;
            virtual void Delete() = 0;
        private:
            size_t size, freq;
            Format format;
        };

        class OALBuffer : public Buffer
        {
        public:
            OALBuffer() = default;
            ~OALBuffer() = default;
            OALBuffer::OALBuffer(const void *data, size_t size, Format format, size_t freq);
            virtual void BufferData(const void* data, size_t size, Format format, size_t freq) override;
            virtual void Delete() override;

        private:
            ALenum getFormat(Format format);

            ALuint id;
        };

        class Listener
        {
        };

        class Source
        {
        };

        class Context
        {
        };

        std::shared_ptr<Buffer> createBuffer(const void *data)
        {
            return std::make_shared<Buffer>();
        }

    }
}