#include "IGame.h"

void IGame::Loop()
{
	auto window_ = Get<Window>();
	auto input_ = Get<Input>();
	auto world_ = Get<World>();
	auto renderer_ = Get<Renderer>();

	std::cout << "Start running" << std::endl;

	while (!window_->ShouldClose())
	{
		// Poll input
		input_->PollEvents();

		// Update world
		world_->Update(0.01f);

		// Render
		renderer_->Render(*world_);

		window_->SwapBuffer();
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
