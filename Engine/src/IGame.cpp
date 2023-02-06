#include "pch.h"
#include "IGame.h"

std::map<std::type_index, std::shared_ptr<System>> IGame::systems_ = std::map<std::type_index, std::shared_ptr<System>>();
std::shared_ptr<IInput> IGame::inputSystem_ = IInput::GetDefault();
std::shared_ptr<IWorld> IGame::worldSystem_ = IWorld::GetDefault();
std::shared_ptr<IWindow> IGame::windowSystem_ = IWindow::GetDefault();
std::shared_ptr<IRenderer> IGame::rendererSystem_ = IRenderer::GetDefault();
std::shared_ptr<IAudio> IGame::audioSystem_ = IAudio::GetDefault();
std::shared_ptr<IHUD> IGame::hudSystem_ = IHUD::GetDefault();

void IGame::Loop()
{
	auto& window = GetWindow();
	auto& input = GetInput();
	auto& world = GetWorld();
	auto& renderer = GetRenderer();
	auto& hud = GetHUD();
	auto& audio_ = IGame::GetAudio();

	// TODO use the real dt
	float dt = 0.01f;

	while (!window.ShouldClose())
	{
		// Poll input
		input.OnTick(dt);

		// Update world
		world.OnTick(dt);

		// Render
		renderer.Render(world);

		audio_.OnTick(dt);

		for (auto& sys : systems_)
			sys.second->OnTick(dt);

		hud.OnTick(dt);

		window.OnTick(dt);
	}
}

void IGame::Stop()
{
	// TODO add cleanup methods here
}

void IGame::Run()
{
	Loop();
	Stop();
}

IWorld& IGame::GetWorld()
{
	if (!worldSystem_)
		worldSystem_ = IWorld::GetDefault();
	return *worldSystem_;
}

void IGame::SetWorld(std::shared_ptr<IWorld> world)
{
	worldSystem_ = world;
	worldSystem_->Init();
}

IWindow& IGame::GetWindow()
{
	if (!windowSystem_)
		windowSystem_ = IWindow::GetDefault();
	return *windowSystem_;
}

void IGame::SetWindow(std::shared_ptr<IWindow> window)
{
	windowSystem_ = window;
	windowSystem_->Init();
}

IInput& IGame::GetInput()
{
	if (!inputSystem_)
		inputSystem_ = IInput::GetDefault();
	return *inputSystem_;
}

void IGame::SetInput(std::shared_ptr<IInput> input)
{
	inputSystem_ = input;
	inputSystem_->Init();
}

IRenderer& IGame::GetRenderer()
{
	if (!rendererSystem_)
		rendererSystem_ = IRenderer::GetDefault();
	return *rendererSystem_;
}

void IGame::SetRenderer(std::shared_ptr<IRenderer> renderer)
{
	rendererSystem_ = renderer;
	rendererSystem_->Init();
}

IAudio& IGame::GetAudio()
{
	if (!audioSystem_)
		audioSystem_ = IAudio::GetDefault();
	return *audioSystem_;
}

void IGame::SetAudio(std::shared_ptr<IAudio> audio)
{
	audioSystem_ = audio;
	audioSystem_->Init();
}

IHUD& IGame::GetHUD()
{
	if (!hudSystem_)
		return *IHUD::GetDefault();
	return *hudSystem_;
}

void IGame::SetHUD(std::shared_ptr<IHUD> hud)
{
	hudSystem_ = hud;
	hudSystem_->Init();
}
