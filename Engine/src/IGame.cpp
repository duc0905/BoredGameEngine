#include "IGame.h"

std::shared_ptr<Input> IGame::inputSystem_ = nullptr;
std::shared_ptr<World> IGame::worldSystem_ = nullptr;
std::shared_ptr<Window> IGame::windowSystem_ = nullptr;
IRenderer& IGame::rendererSystem_ = IRenderer::GetDefault();

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
		input_.PollEvents();

		// Update world
		world_.Update(0.01f);

		// Render
		renderer_.Render(world_);

		window_.SwapBuffer();
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

World& IGame::GetWorld()
{
	return *worldSystem_;
}

void IGame::SetWorld(World* world)
{
	worldSystem_ = std::shared_ptr<World>(world);
}

Window& IGame::GetWindow()
{
	return *windowSystem_;
}

void IGame::SetWindow(Window* window)
{
	windowSystem_ = std::shared_ptr<Window>(window);
}

Input& IGame::GetInput()
{
	return *inputSystem_;
}

void IGame::SetInput(Input* input)
{
	inputSystem_ = std::shared_ptr<Input>(input);
}

IRenderer& IGame::GetRenderer()
{
	return *rendererSystem_;
}

void IGame::SetRenderer(IRenderer* renderer)
{
	rendererSystem_ = std::shared_ptr<IRenderer>(renderer);
}
