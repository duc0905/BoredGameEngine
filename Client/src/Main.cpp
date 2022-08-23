#include "IGame.h"
#include "Window/GLFWWindow.h"
#include "World/World.h"
#include "Input/GLFWInput.h"

#include "Actor/OrthoCamera.h"

#include "MyRenderer.h"
#include "CubeActor.h"

int main()
{
	auto world = std::make_shared<World>();
	auto window = std::make_shared<GLFWWindow>(800, 800, "Bored Chess");
	auto renderer = std::make_shared<MyRenderer>();

	IGame::SetWorld(world);
	IGame::SetWindow(window);
	IGame::SetRenderer(renderer);

	// My design sucks so this happens
	auto input = GLFWInput::GetInstancePtr();
	IGame::SetInput(input);

	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	world->AddActor(cube);

	std::shared_ptr<OrthoCamera> cam = std::make_shared<OrthoCamera>(glm::vec4(800, -800, 800, -800));
	world->AddActor(cam);
	renderer->UseCamera(cam);

	cam->FindComponent<TransformComponent>()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f));

	IGame::Run();

	return 0;
}