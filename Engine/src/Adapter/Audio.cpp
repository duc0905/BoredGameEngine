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

		void OALBuffer::Delete()
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

	} // namespace Audio
} // namespace Bored