#include "IGame.h"
#include "Window/GLFWWindow.h"
#include "World/World.h"
#include "Input/GLFWInput.h"
#include "Audio/OpenALAudio.h"

#include "Actor/OrthoCamera.h"

#include "MyRenderer.h"
#include "CubeActor.h"
#include "ChessGameMode.h"

int main()
{
	auto window = std::make_shared<GLFWWindow>(800, 800, "Bored Chess");
	auto renderer = std::make_shared<MyRenderer>();

	IGame::SetWindow(window);
	IGame::SetRenderer(renderer);


	//// My design sucks so this happens
	auto input = GLFWInput::GetInstancePtr();
	IGame::SetInput(input);

	auto world = std::make_shared<World>();
	IGame::SetWorld(world);

	auto audio = std::make_shared<OpenALAudio>();
	IGame::SetAudio(audio);
	OpenALBuffer buffer;
	if (!buffer.LoadWav("sounds/bensound-sunny.wav"))
	{
		LOG_COLOR("Fuck", COLOR::RED, COLOR::BLACK);
	}
	OpenALSource source;
	source.BindBuffer(buffer.GetId());
	source.SetGain(1.0f);
	source.SetPosition({ 0.0f, 0.0f, 0.0f });
	source.SetVelocity({ 0.0f, 0.0f, 0.0f });
	audio->SetListenerPosition({ 0.0f, 0.0f, 0.0f });
	audio->SetListenerVelocity({ 0.0f, 0.0f, 0.0f });
	source.Play();

	world->UseGameMode<ChessGameMode>(*world);

	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	world->AddActor(cube);

	std::shared_ptr<OrthoCamera> cam = std::make_shared<OrthoCamera>(glm::vec4(800, -800, 800, -800));
	world->AddActor(cam);
	renderer->UseCamera(cam);

	cam->FindComponent<TransformComponent>()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f));

	IGame::Run();

	return 0;
}