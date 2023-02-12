#include "IGame.h"
#include "Window/GLFWWindow.h"
#include "World/World.h"
#include "Input/GLFWInput.h"
#include "MyRenderer.h"
#include "MyAudio.h"
#include "HUD/ImGuiHUD.h"
//#include "demos/ImGuiHUD.h"

#include "Actor/OrthoCamera.h"
#include "Actor/PerspectiveCamera.h"

#include "demos/BasicCamera.h"
#include "demos/Lighting.h"
#include "demos/Chess/Chess.h"
#include "demos/DebugCoordinates.h"

void SetupSystems();

int main()
{
	SetupSystems();
	BasicCameraDemo();

	LightingDemo();

	ChessDemo();

	DebugCoordDemo();

	IGame::Run();

	return 0;
}

void SetupSystems()
{
	auto window = std::make_shared<GLFWWindow>(800, 800, "Bored Chess");
	IGame::SetWindow(window);

	auto renderer = std::make_shared<Renderer>();
	IGame::SetRenderer(renderer);

	//// My design sucks so this happens
	auto input = GLFWInput::GetInstancePtr();
	IGame::SetInput(input);

	// HUD
	auto hud = std::make_shared<ImGuiHUD>();
	IGame::SetHUD(hud);

	auto world = std::make_shared<World>();
	IGame::SetWorld(world);

	auto audio = std::make_shared<MyAudio>();
	IGame::SetAudio(audio);
}
