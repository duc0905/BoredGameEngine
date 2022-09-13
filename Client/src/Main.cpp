#include "IGame.h"
#include "Window/GLFWWindow.h"
#include "World/World.h"
#include "Input/GLFWInput.h"
#include "MyRenderer.h"
#include "MyAudio.h"
#include "demos/ImGuiHUD.h"

#include "Actor/OrthoCamera.h"

#include "CubeActor.h"
#include "ChessGameMode.h"

int main()
{
	auto window = std::make_shared<GLFWWindow>(800, 800, "Bored Chess");
	IGame::SetWindow(window);

	auto renderer = std::make_shared<Renderer>();
	IGame::SetRenderer(renderer);

	//// My design sucks so this happens
	auto input = GLFWInput::GetInstancePtr();
	IGame::SetInput(input);

	auto world = std::make_shared<World>();
	IGame::SetWorld(world);

	auto audio = std::make_shared<MyAudio>();
	IGame::SetAudio(audio);

	world->UseGameMode<ChessGameMode>(*world);

	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	world->AddActor(cube);

	std::shared_ptr<OrthoCamera> cam = std::make_shared<OrthoCamera>(glm::vec4(800, -800, 800, -800));
	world->AddActor(cam);
	renderer->UseCamera(cam);

	std::string actionKey = "buonce";

	std::shared_ptr<Context> myContext = std::make_shared<Context>();
	myContext->AddActionMapping(KeyInput::KEY_B, 0, actionKey);
	myContext->AddRangeMapping(KeyInput::KEY_P, 0, "yeet_RANGE", 1);
	myContext->AddRangeMapping(KeyInput::KEY_X ,0, "rX", 1);
	myContext->AddRangeMapping(KeyInput::KEY_Y ,0, "rY", 1);
	myContext->AddRangeMapping(KeyInput::KEY_Z ,0,"rZ", 1);
	myContext->AddRangeMapping(KeyInput::KEY_X ,0, "-rX", 1);
	myContext->AddRangeMapping(KeyInput::KEY_Y ,0, "-rY", 1);
	myContext->AddRangeMapping(KeyInput::KEY_Z, 0,"-rZ", 1);
	input->AddContext(myContext);

	cam->FindComponent<TransformComponent>()->Translate(glm::vec3(-10.0f, 0.0f, 0.0f));
	//cam->FindComponent<TransformComponent>()->Rotate(glm::vec3(glm::pi<float>() / 2.f, 0.0f, 0.0f));

	IGame::Run();

	return 0;
}