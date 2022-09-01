#include "MyAudio.h"
#include "IGame.h"

MyAudio::MyAudio()
{
	bounceBuffer = OpenALBuffer();
	if (!bounceBuffer.LoadWav("sounds/bounce.wav"))
	{
		LOG_COLOR("Fuck", COLOR::RED, COLOR::BLACK);
	}
	mySource = OpenALSource();
	mySource.BindBuffer(bounceBuffer.GetId());
	mySource.SetGain(1.0f);
	mySource.SetPosition({ 0.0f, 0.0f, 0.0f });
	mySource.SetVelocity({ 0.0f, 0.0f, 0.0f });
	SetListenerPosition({ 0.0f, 0.0f, 0.0f });
	SetListenerVelocity({ 0.0f, 0.0f, 0.0f });

}

void MyAudio::Init()
{
	auto& input = IGame::GetInput();
	std::string actionKey = "PLAY_BOUND_SOUND";

	std::shared_ptr<Context> myContext = std::make_shared<Context>();
	myContext->AddActionMapping(KeyInput::KEY_B, 0, actionKey);
	input.AddContext(myContext);
	input.BindAction(actionKey, [this](KeyInput::Action action) -> void {
		if (action == KeyInput::PRESS)
		{
			mySource.Play();
			LOG_COLOR("PRESS B", COLOR::GREEN, COLOR::BLACK);
		}
	});
}
