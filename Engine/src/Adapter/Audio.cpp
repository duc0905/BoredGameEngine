#include "Audio.h" // Path: Audio.h
#include <iostream>

namespace Bored
{
	namespace Audio
	{
		OALBuffer::OALBuffer(const void *data, size_t size, Format format, size_t freq)
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

		OALBuffer::~OALBuffer()
		{
			alDeleteBuffers(1, &id);
		}

		ALenum OALBuffer::getFormat(Format format)
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

		int OALBuffer::GetFreq(){
			int value;
			alGetBufferi(id, AL_FREQUENCY, &value);
			return value;
		}

		int OALBuffer::GetBits(){
			int value;
			alGetBufferi(id, AL_BITS, &value);
			return value;
		}
		int OALBuffer::GetChannels(){
			int value;
			alGetBufferi(id, AL_CHANNELS, &value);
			return value;
		}
		int OALBuffer::GetSize(){
			int value;
			alGetBufferi(id, AL_SIZE, &value);
			return value;
		}
		std::vector<int> OALBuffer::GetData(){
			throw std::runtime_error("Get data not supported");
		}

		void OALSource::SetPosition(const glm::vec3& vec) {
			alSource3f(source, AL_POSITION, vec[0], vec[1], vec[2]);
		}
		void OALSource::SetVelocit(const glm::vec3& vec) {
			alSource3f(source, AL_VELOCITY, vec[0], vec[1], vec[2]);
		}
		void OALSource::SetDirection(const glm::vec3& vec) {
			alSource3f(source, AL_DIRECTION, vec[0], vec[1], vec[2]);
		}

		void OALSource::SetPitch(float value) {
			alSourcef(source, AL_PITCH, value);
		}
		void OALSource::SetGain(float value) {
			alSourcef(source, AL_GAIN, value);
		}
		void OALSource::SetMinGain(float value) {
			alSourcef(source, AL_MIN_GAIN, value);
		}
		void OALSource::SetMaxGain(float value) {
			alSourcef(source, AL_MAX_GAIN, value);
		}
		void OALSource::SetMaxDistance(float value) {
			alSourcef(source, AL_MAX_DISTANCE, value);
		}
		void OALSource::SetRolloffFactor(float value) {
			alSourcef(source, AL_ROLLOFF_FACTOR, value);
		}
		void OALSource::SetConeOuterGain(float value) {
			alSourcef(source, AL_CONE_OUTER_GAIN, value);
		}
		void OALSource::SetConeInnerAngle(float value) {
			alSourcef(source, AL_CONE_INNER_ANGLE, value);
		}
		void OALSource::SetConeOuterAngle(float value) {
			alSourcef(source, AL_CONE_OUTER_ANGLE, value);
		}
		void OALSource::SetReferenceDistance(float value) {
			alSourcef(source, AL_REFERENCE_DISTANCE, value);
		}

		void OALSource::SetSourceRelative(unsigned int value) {
			alSourcei(source, AL_SOURCE_RELATIVE, value);
		};
		void OALSource::SetLooping(unsigned int value) {
			alSourcei(source, AL_LOOPING, value);
		};
		void OALSource::SetBuffer(unsigned int value) {
			alSourcei(source, AL_BUFFER, value);
		};
		void OALSource::SetSourceState(unsigned int value) {
			alSourcei(source, AL_SOURCE_STATE, value);
		};
	} // namespace Audio
} // namespace Bored