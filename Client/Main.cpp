//#include "MyGame.h"
#include "../src/IGame.h"
#include "../src/Actor//OrthoCamera.h"
#include "CubeActor.h"
#include "../src/Input/Context.h"
#include "../src/Input/Constant.h"

//class MyAlocator : public Alocator<A>
//{};

void ContextTesting()
{
	std::cout << "Running Context Testing Suites" << std::endl;
	std::cout << "Testing Initilization" << std::endl;
	Context* con = new Context();
	con->Debuggin();
	std::cout << "Adding Actions" << std::endl;
	con->AddActionMapping(KeyInput::KEY_1, KeyInput::MOD_SHIFT, "Action1");
	con->AddActionMapping(KeyInput::KEY_2, KeyInput::MOD_SHIFT, "Action2");
	con->AddActionMapping(KeyInput::KEY_3, KeyInput::MOD_SHIFT, "Action3");
	con->AddActionMapping(KeyInput::KEY_3, KeyInput::MOD_CTRL, "Action4");
	std::cout << "Adding Ranges" << std::endl;
	con->AddRangeMapping(KeyInput::KEY_1, KeyInput::MOD_SHIFT, "Range1", 0.3);
	con->AddRangeMapping(KeyInput::KEY_2, KeyInput::MOD_CTRL, "Range2", 1.3);
	con->AddRangeMapping(KeyInput::KEY_3, KeyInput::MOD_SHIFT, "Range3", 0);
	con->Debuggin();
	std::cout << "Adding pre-existing actions" << std::endl;
	con->AddActionMapping(KeyInput::KEY_1, KeyInput::MOD_SHIFT, "New_action_1");
	con->AddActionMapping(KeyInput::KEY_2, KeyInput::MOD_SHIFT, "New_action_2");
	std::cout << "Adding pre-existing ranges" << std::endl;
	con->AddRangeMapping(KeyInput::KEY_3, KeyInput::MOD_SHIFT, "New_Range3", 10);
	con->AddRangeMapping(KeyInput::KEY_1, KeyInput::MOD_SHIFT, "NEW_Range1", 0);
	con->Debuggin();
}

int RendererTesting()
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

	return 0;
}

int main()
{
	MyGame game;
	game.Run();
}