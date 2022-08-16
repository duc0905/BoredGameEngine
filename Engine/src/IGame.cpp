#include "pch.h"
#include "IGame.h"

std::shared_ptr<IInput> IGame::inputSystem_ = IInput::GetDefault();
std::shared_ptr<IWorld> IGame::worldSystem_ = IWorld::GetDefault();
std::shared_ptr<IWindow> IGame::windowSystem_ = IWindow::GetDefault();
std::shared_ptr<IRenderer> IGame::rendererSystem_ = IRenderer::GetDefault();

void IGame::Loop()
{
	auto& window_ = IGame::GetWindow();
	auto& input_ = IGame::GetInput();
	auto& world_ = IGame::GetWorld();
	auto& renderer_ = IGame::GetRenderer();

	std::cout << "Start running" << std::endl;

	while (!window_.ShouldClose())
	{
		// Poll input
		input_.OnTick(0.01f);

		// Update world
		world_.OnTick(0.01f);

		// Render
		renderer_.Render(world_);

		window_.OnTick(0.01f);
	}
}

void IGame::Stop()
{
}

void IGame::Run()
{
	Loop();
	Stop();
}

IWorld& IGame::GetWorld()
{
	return *worldSystem_;
}

void IGame::SetWorld(IWorld* world)
{
	worldSystem_ = std::shared_ptr<IWorld>(world);
}

void IGame::SetWorld(std::shared_ptr<IWorld> world)
{
	worldSystem_ = world;
}

IWindow& IGame::GetWindow()
{
	return *windowSystem_;
}

void IGame::SetWindow(IWindow* window)
{
	windowSystem_ = std::shared_ptr<IWindow>(window);
}

void IGame::SetWindow(std::shared_ptr<IWindow> window)
{
	windowSystem_ = window;
}

IInput& IGame::GetInput()
{
	return *inputSystem_;
}

void IGame::SetInput(IInput* input)
{
	inputSystem_ = std::shared_ptr<IInput>(input);
}

void IGame::SetInput(std::shared_ptr<IInput> input)
{
	inputSystem_ = input;
}

IRenderer& IGame::GetRenderer()
{
	if (!rendererSystem_)
		return *IRenderer::GetDefault();
	return *rendererSystem_;
}

void IGame::SetRenderer(IRenderer* renderer)
{
	rendererSystem_ = std::shared_ptr<IRenderer>(renderer);
}

void IGame::SetRenderer(std::shared_ptr<IRenderer> renderer)
{
	rendererSystem_ = renderer;
}
