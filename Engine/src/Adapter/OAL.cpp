#include "OAL.h" // Path: Audio.h
#include <memory>
#include <AL/alc.h>
#include <AL/al.h>
#include <iostream>

namespace Bored
{
	namespace Audio
	{
        namespace OAL {
            Buffer::Buffer(const void *data, size_t size, Format format, size_t freq)
            {
                alGenBuffers(1, &id);
                ALenum error;
                if ((error = alGetError()) != AL_NO_ERROR)
                {
                    std::cerr << "alGenBuffers :" << error << std::endl;
                    return;
                }
                alBufferData(id, getFormat(format), data, size, freq);
            }

            Buffer::~Buffer()
            {
                alDeleteBuffers(1, &id);
            }

            ALenum Buffer::getFormat(Format format)
            {
                switch (format)
                {
                    case MONO8:
                        return AL_FORMAT_MONO8;
                    case MONO16:
                        return AL_FORMAT_MONO16;
                    case STEREO8:
                        return AL_FORMAT_STEREO8;
                    case STEREO16:
                        return AL_FORMAT_STEREO16;
                    default:
                        return AL_FORMAT_STEREO16;
                }
            }

            int Buffer::GetFreq(){
                int value;
                alGetBufferi(id, AL_FREQUENCY, &value);
                return value;
            }

            int Buffer::GetBits(){
                int value;
                alGetBufferi(id, AL_BITS, &value);
                return value;
            }
            int Buffer::GetChannels(){
                int value;
                alGetBufferi(id, AL_CHANNELS, &value);
                return value;
            }
            int Buffer::GetSize(){
                int value;
                alGetBufferi(id, AL_SIZE, &value);
                return value;
            }
            std::vector<int> Buffer::GetData(){
                throw std::runtime_error("Get data not supported");
            }

            Source::Source() {
                alGenSources(1, &id);
            }

            void Source::SetPosition(const glm::vec3& vec) {
                alSource3f(id, AL_POSITION, vec[0], vec[1], vec[2]);
            }
            void Source::SetVelocity(const glm::vec3& vec) {
                alSource3f(id, AL_VELOCITY, vec[0], vec[1], vec[2]);
            }
            void Source::SetDirection(const glm::vec3& vec) {
                alSource3f(id, AL_DIRECTION, vec[0], vec[1], vec[2]);
            }

            void Source::SetPitch(float value) {
                alSourcef(id, AL_PITCH, value);
            }
            void Source::SetGain(float value) {
                alSourcef(id, AL_GAIN, value);
            }
            void Source::SetMinGain(float value) {
                alSourcef(id, AL_MIN_GAIN, value);
            }
            void Source::SetMaxGain(float value) {
                alSourcef(id, AL_MAX_GAIN, value);
            }
            void Source::SetMaxDistance(float value) {
                alSourcef(id, AL_MAX_DISTANCE, value);
            }
            void Source::SetRolloffFactor(float value) {
                alSourcef(id, AL_ROLLOFF_FACTOR, value);
            }
            void Source::SetConeOuterGain(float value) {
                alSourcef(id, AL_CONE_OUTER_GAIN, value);
            }
            void Source::SetConeInnerAngle(float value) {
                alSourcef(id, AL_CONE_INNER_ANGLE, value);
            }
            void Source::SetConeOuterAngle(float value) {
                alSourcef(id, AL_CONE_OUTER_ANGLE, value);
            }
            void Source::SetReferenceDistance(float value) {
                alSourcef(id, AL_REFERENCE_DISTANCE, value);
            }
            void Source::SetSourceRelative(unsigned int value) {
                alSourcei(id, AL_SOURCE_RELATIVE, value);
            };
            void Source::SetLooping(unsigned int value) {
                alSourcei(id, AL_LOOPING, value);
            };
            void Source::SetBuffer(unsigned int value) {
                alSourcei(id, AL_BUFFER, value);
            };
            void Source::SetSourceState(unsigned int value) {
                alSourcei(id, AL_SOURCE_STATE, value);
            };

            void Source::AddBuffer(std::shared_ptr<Bored::Audio::Buffer> buf) {
                std::shared_ptr<Buffer> oalBuf;
                if ((oalBuf = std::dynamic_pointer_cast<Buffer>(buf)) != nullptr) {
                    buffer = buf;
                    bufferId = oalBuf->GetId();
                    alSourcei(id, AL_BUFFER, (int)oalBuf->GetId());
                } else {
                    throw new std::exception("OpenAL source only works with OpenAL Buffers");
                }
            }

            void Listener::SetGain(float val) {
                gain = val;
                alListenerf(AL_GAIN, val);
            }

            void Listener::SetPosition(const glm::vec3& val) {
                position = val;
                alListener3f(AL_POSITION, val.x, val.y, val.z);
            }

            void Listener::SetVelocity(const glm::vec3& val) {
                position = val;
                alListener3f(AL_VELOCITY, val.x, val.y, val.z);
            }

            void Listener::SetAtDir(const glm::vec3& val) {
                atDir = val;
                float ori[6] = {atDir.x, atDir.y, atDir.z, upDir.x, upDir.y, upDir.z};
                alListenerfv(AL_ORIENTATION, ori);
            }

            void Listener::SetUpDir(const glm::vec3& val) {
                upDir = val;
                float ori[6] = {atDir.x, atDir.y, atDir.z, upDir.x, upDir.y, upDir.z};
                alListenerfv(AL_ORIENTATION, ori);
            }

            float Listener::GetGain() const { return gain; }
            glm::vec3 Listener::GetPosition() const { return position; }
            glm::vec3 Listener::GetVelocity() const { return velocity; }
            glm::vec3 Listener::GetAtDir() const { return atDir; }
            glm::vec3 Listener::GetUpDir() const { return upDir; }

            Context::Context() : Bored::Audio::Context(std::make_shared<Listener>())
            {
            }

            Context::~Context() {
                // TODO implement
            }

            std::shared_ptr<Bored::Audio::Source> Context::CreateSource() {
                return std::make_shared<Source>();
            }

        } // namespace OAL
    }// namespace Audio
} // namespace Bored
