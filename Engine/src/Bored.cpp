// int main(int n, char** args)
// {
//     AudioFile<double> audio;
//     std::string wavFilePath = "../resource/example.wav";
//     ALCdevice* device = alcOpenDevice(nullptr);
//     if (!device) {
//         std::cerr << "Failed to open OpenAL device" << std::endl;
//         return 1;
//     }

//     ALCcontext* context = alcCreateContext(device, nullptr);
//     if (!context) {
//         std::cerr << "Failed to create OpenAL context" << std::endl;
//         alcCloseDevice(device);
//         return 1;
//     }

//     if (!alcMakeContextCurrent(context)) {
//         std::cerr << "Failed to make OpenAL context current" << std::endl;
//         alcDestroyContext(context);
//         alcCloseDevice(device);
//         return 1;
//     }

//     // Load WAV file
//     ALuint buffer;
//     alGenBuffers(1, &buffer);

//     ALenum format;
//     ALsizei size;
//     ALsizei frequency;
//     ALvoid* data;
//     ALboolean loop = AL_FALSE;

//     // loadWAVFile((ALbyte*)wavFilePath.c_str(), &format, &data, &size, &frequency, &loop);
//     audio.load(wavFilePath);
//     frequency = audio.getSampleRate();

//     audio.samples.size() * audio.samples[0].size() * sizeof(double);

//     if (audio.isMono()) {
//       if (audio.getBitDepth() == 8)
//         format = AL_FORMAT_MONO8;
//       else
//         format = AL_FORMAT_MONO16;
//     }
//     else if (audio.isStereo()) {
//       if (audio.getBitDepth() == 8)
//         format = AL_FORMAT_STEREO8;
//       else
//         format = AL_FORMAT_STEREO16;
//     }

//     alBufferData(buffer, format, data, size, frequency);

//     // Create a source
//     ALuint source;
//     alGenSources(1, &source);
//     alSourcei(source, AL_BUFFER, buffer);

//     // Play the source
//     alSourcePlay(source);

//     // Wait for the sound to finish playing
//     ALint sourceState;
//     do {
//         alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
//     } while (sourceState == AL_PLAYING);

//     // Clean up resources
//     alDeleteSources(1, &source);
//     alDeleteBuffers(1, &buffer);

//     // Destroy OpenAL context and device
//     alcMakeContextCurrent(nullptr);
//     alcDestroyContext(context);
//     alcCloseDevice(device);

//     return 0;
// }
