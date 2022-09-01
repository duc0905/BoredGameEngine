#include "pch.h"
#include <AudioFile.h>
#include "OpenALBuffer.h"

OpenALBuffer::OpenALBuffer()
{
    alGenBuffers(1, &id_);
}

const unsigned int OpenALBuffer::GetId() const
{
    return id_;
}

bool OpenALBuffer::LoadWav(const std::string& filepath)
{
    AudioFile<float> file;
    file.load("res/" + filepath);
    ALenum format;
    if (file.isMono())
    {
        switch (file.getBitDepth())
        {
        case 8:
            format = AL_FORMAT_MONO8;
            break;
        case 16:
            format = AL_FORMAT_MONO16;
            break;
        default:
            return false;
        }
    }
    else if (file.isStereo())
    {
        switch (file.getBitDepth())
        {
        case 8:
            format = AL_FORMAT_STEREO8;
            break;
        case 16:
            format = AL_FORMAT_STEREO16;
            break;
        default:
            return false;
        }
    }
    else
        return false;
    alBufferData(
        id_, 
        format, 
        &file.samples[0][0],
        file.samples.size() * sizeof(file.samples[0][0]),
        file.getSampleRate());
    return true;
}

bool OpenALBuffer::CleanUp()
{
    alDeleteBuffers(1, &id_);
    return true;
}
