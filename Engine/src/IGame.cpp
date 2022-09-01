#include "pch.h"
#include "IGame.h"

std::shared_ptr<IInput> IGame::inputSystem_ = IInput::GetDefault();
std::shared_ptr<IWorld> IGame::worldSystem_ = IWorld::GetDefault();
std::shared_ptr<IWindow> IGame::windowSystem_ = IWindow::GetDefault();
std::shared_ptr<IRenderer> IGame::rendererSystem_ = IRenderer::GetDefault();
std::shared_ptr<IAudio> IGame::audioSystem_ = IAudio::GetDefault();

void IGame::Loop()
{
	auto& window_ = IGame::GetWindow();
	auto& input_ = IGame::GetInput();
	auto& world_ = IGame::GetWorld();
	auto& renderer_ = IGame::GetRenderer();
	auto& audio_ = IGame::GetAudio();

	std::cout << "Start running" << std::endl;

	while (!window_.ShouldClose())
	{
		// Poll input
		input_.OnTick(0.01f);

		// Update world
		world_.OnTick(0.01f);

		// Render
		renderer_.Render(world_);

		audio_.OnTick(0.01f);

		window_.OnTick(0.01f);
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
