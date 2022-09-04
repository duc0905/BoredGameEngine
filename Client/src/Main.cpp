#include "IGame.h"
#include "Window/GLFWWindow.h"
#include "World/World.h"
#include "Input/GLFWInput.h"
#include "MyRenderer.h"

#include "demos/ImGuiHUD.h"

#include "Actor/OrthoCamera.h"

#include "CubeActor.h"
#include "ChessGameMode.h"

int main()
{
	auto window = std::make_shared<GLFWWindow>(800, 800, "Bored Chess");
	IGame::SetWindow(window);

	auto renderer = std::make_shared<MyRenderer>();
	IGame::SetRenderer(renderer);

	//// My design sucks so this happens
	auto input = GLFWInput::GetInstancePtr();
	IGame::SetInput(input);

	auto world = std::make_shared<World>();
	IGame::SetWorld(world);
	world->UseGameMode<ChessGameMode>(*world);

	std::shared_ptr<Actor> cube = std::make_shared<CubeActor>();
	world->AddActor(cube);

	std::shared_ptr<OrthoCamera> cam = std::make_shared<OrthoCamera>(glm::vec4(800, -800, 800, -800));
	world->AddActor(cam);
	renderer->UseCamera(cam);

	std::shared_ptr<Context> cameraContext = std::make_shared<Context>();
	cameraContext->AddRangeMapping(KeyInput::KEY_A, 0, "MOVE_CAMERA_X", -1.0f);
	cameraContext->AddRangeMapping(KeyInput::KEY_D, 0, "MOVE_CAMERA_X", 1.0f);
	cameraContext->AddRangeMapping(KeyInput::KEY_W, 0, "MOVE_CAMERA_Y", 1.0f);
	cameraContext->AddRangeMapping(KeyInput::KEY_S, 0, "MOVE_CAMERA_Y", -1.0f);
	cameraContext->AddActionMapping(KeyInput::KEY_MB_2, 0, "DIT COME");
	cameraContext->AddRangeMapping(KeyInput::MOUSE_SCROLL_X, 0, "DIT SCROLL", 1.0f);

	input->AddContext(cameraContext);
	input->ActivateContext(cameraContext);

	input->BindRange("MOVE_CAMERA_X", [=](KeyInput::Action action, float weight) -> void {
		if (action == KeyInput::PRESS || action == KeyInput::REPEAT) {
			auto trans = cam->FindComponent<TransformComponent>();
			trans->Translate(glm::vec3(0.0f, weight, 0.0f));
			LOG(trans->GetTranslation().y);
		}
		});

	input->BindRange("MOVE_CAMERA_Y", [=](KeyInput::Action action, float weight) -> void {
		if (action == KeyInput::PRESS || action == KeyInput::REPEAT) {
			auto trans = cam->FindComponent<TransformComponent>();
			trans->Translate(glm::vec3(0.0f, 0.0f, weight));
			LOG(trans->GetTranslation().z);
		}
		});

	input->BindAction("DIT COME", [](KeyInput::Action action) -> void {
		LOG("DIT COME");
		});
	input->BindRange("DIT SCROLL", [](KeyInput::Action action, float val) -> void {
		LOG(val);
		});

	cam->FindComponent<TransformComponent>()->Translate(glm::vec3(-3.0f, 0.0f, 0.0f));

	ImGuiHUDDemo();

	IGame::Run();

	return 0;
}