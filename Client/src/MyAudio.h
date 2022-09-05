#pragma once
#include "Audio/OpenALAudio.h"

class MyAudio : public OpenALAudio
{
private:
	OpenALSource mySource;
	OpenALBuffer bounceBuffer;
public:
	MyAudio();

	virtual void Init() override;
};

