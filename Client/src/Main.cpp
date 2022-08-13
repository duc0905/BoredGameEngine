#include "IGame.h"
#include "Actor/OrthoCamera.h"
#include "CubeActor.h"
#include "Input/Context.h"
#include "Input/Constant.h"
#include "Input/GLFWInput.h"
#include "Renderer/Renderer.h"
#include "Components/MeshComponent.h"

void testingActionRelease() {
	std::cout << "Binding Release call" << std::endl;
}

void testingActionRepeat() {
	std::cout << "Binding Repeat call" << std::endl;
}

void testingAction(KeyInput::Action action) {
	switch (action) {
	case KeyInput::RELEASE: testingActionRelease();
		break;
	case KeyInput::REPEAT: testingActionRepeat();
		break;
	}
}

void InputTesting()
{
	auto* window = new GLFWWindow(800, 600, "Ourscraft");
	window->Init();
	IGame::SetWindow(window);

	std::cout << "Running Input Testing Suites" << std::endl;

	auto& input = GLFWInput::GetInstance();
	auto con1 = std::shared_ptr<Context>(new Context());
	auto con2 = std::shared_ptr<Context>(new Context());
	auto con3 = std::shared_ptr<Context>(new Context());
	con1->AddActionMapping(KeyInput::KEY_A, 0, "String1 1");
	con1->AddActionMapping(KeyInput::KEY_B, 0, "String1 2");
	con1->AddActionMapping(KeyInput::KEY_C, 0, "String1 3");
	con1->AddRangeMapping(KeyInput::KEY_Y, 0, "rY", 1);
	con1->AddRangeMapping(KeyInput::KEY_X, 0, "rX", 1);
	con1->AddRangeMapping(KeyInput::KEY_Z, 0, "rZ", 1);
	con1->AddRangeMapping(KeyInput::KEY_Y, KeyInput::CTRL, "-rY", -1.0f);
	con1->AddRangeMapping(KeyInput::KEY_X, KeyInput::CTRL, "-rX", -1.0f);
	con1->AddRangeMapping(KeyInput::KEY_Z, KeyInput::CTRL, "-rZ", -1.0f);
	input.AddContext(con1);
	input.ActivateContext(con1);
	input.BindAction("String1 1", testingAction);

	auto* renderer = new Renderer();
	renderer->Init();
	IGame::SetRenderer(renderer);

	IGame::SetInput(GLFWInput::GetInstancePtr());

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

int main()
{
	InputTesting();
	return 0;
}