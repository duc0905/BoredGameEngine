#include "IGame.h"
#include "Actor/OrthoCamera.h"
#include "CubeActor.h"
#include "Input/Context.h"
#include "Input/Constant.h"
#include "Input/Input.h"

void InputTesting()
{
	auto* window = new Window(800, 600, "Ourscraft");
	window->Init();
	IGame::SetWindow(window);

	std::cout << "Running Input Testing Suites" << std::endl;
	auto input = Input::GetInstance();
	auto con1 = std::shared_ptr<Context>(new Context());
	auto con2 = std::shared_ptr<Context>(new Context());
	auto con3 = std::shared_ptr<Context>(new Context());
	con1->AddActionMapping(KeyInput::KEY_A, 0, "String1 1");
	con1->AddActionMapping(KeyInput::KEY_B, 0, "String1 2");
	con1->AddActionMapping(KeyInput::KEY_C, 0, "String1 3");
	input->AddContext(con1);
	input->ActivateContext(con1);
	input->EvaluateKey(KeyInput::KEY_A, KeyInput::REPEAT, 0);

	auto* renderer = new Renderer();
	renderer->Init();
	IGame::SetRenderer(renderer);

	IGame::SetInput(input.get());

	auto* world = new World();
	world->Init();
	IGame::SetWorld(world);

	std::shared_ptr<BaseCamera> camera = std::make_shared<OrthoCamera>(glm::vec4(-800, 800, -600, 600));
	auto transcomp = camera->FindComponent<TransformComponent>();
	transcomp->SetTranslation({ -5.0f, 0.0f, 0.0f });
	renderer->UseCamera(camera);

	// std::shared_ptr<Actor> cube = std::make_shared<Actor>();
	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	auto meshC = cube->CreateComponent<MeshComponent>();
	meshC->LoadMesh("cube.obj");

	world->AddActor(cube);

	IGame::Run();
}

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
	std::cout << "Removing actions" << std::endl;
	con->RemoveActionMapping(KeyInput::KEY_1, KeyInput::MOD_SHIFT);
	con->RemoveActionMapping(KeyInput::KEY_1, KeyInput::MOD_SHIFT);
	con->RemoveActionMapping(KeyInput::KEY_2, KeyInput::MOD_SHIFT);
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

	auto input = Input::GetInstance();
	input->Init();
	IGame::SetInput(input.get());

	auto* world = new World();
	world->Init();
	IGame::SetWorld(world);

	std::shared_ptr<BaseCamera> camera = std::make_shared<OrthoCamera>(glm::vec4(- 800, 800, -600, 600));
	auto transcomp = camera->FindComponent<TransformComponent>();
	transcomp->SetTranslation({-5.0f, 0.0f, 0.0f});
	renderer->UseCamera(camera);

	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	auto meshC = cube->CreateComponent<MeshComponent>();
	meshC->LoadMesh("cube.obj");

	world->AddActor(cube);

	IGame::Run();

	return 0;
}

int main()
{
	InputTesting();
	return 0;
}