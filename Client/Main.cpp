//#include "MyGame.h"
#include "../src/IGame.h"
#include "../src/Actor//OrthoCamera.h"
#include "CubeActor.h"

//class MyAlocator : public Alocator<A>
//{};

int main()
{
	auto* window = new Window(800, 600, "Ourscraft");
	window->Init();
	IGame::SetWindow(window);

	auto* renderer = new Renderer();
	renderer->Init();
	IGame::SetRenderer(renderer);

	auto* input = new Input(*window);
	input->Init();
	IGame::SetInput(input);

	auto* world = new World();
	world->Init();
	IGame::SetWorld(world);

	std::shared_ptr<BaseCamera> camera = std::make_shared<OrthoCamera>(glm::vec4(- 800, 800, -600, 600));
	auto transcomp = camera->FindComponent<TransformComponent>();
	transcomp->SetTranslation({-5.0f, 0.0f, 0.0f});
	renderer->UseCamera(camera);

	// std::shared_ptr<Actor> cube = std::make_shared<Actor>();
	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	auto meshC = cube->CreateComponent<MeshComponent>();
	meshC->LoadMesh("cube.obj");

	world->AddActor(cube);

	IGame::Run();
}