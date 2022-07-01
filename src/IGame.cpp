#include "IGame.h"

void IGame::Loop()
{
	Window& window_ = Get<Window>();
	Input& input_ = Get<Input>();
	World& world_ = Get<World>();
	Renderer& renderer_ = Get<Renderer>();

	std::cout << "Start running" << std::endl;

	while (!Get<Window>().ShouldClose())
	{
		// Poll input
		input_.PollEvents();

		// Update world
		world_.Update(0.01f);

		// Render
		renderer_.Render();

		window_.SwapBuffer();
	}
}

void IGame::Run()
{
	Init();
	Loop();
	Stop();
}

IGame::~IGame()
{
}
