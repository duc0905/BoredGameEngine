#include "IGame.h"

void IGame::Loop()
{
	while (!Window_->ShouldClose())
	{
		// Poll input
		Input_->PollEvents();

		// Update world
		World_->Update(0.01f);

		// Render
		Renderer_->Render();

		Window_->SwapBuffer();
	}
}

void IGame::Run()
{
	Init();
	Loop();
	Stop();
}

void IGame::CreateWorld()
{
	World_ = std::make_unique<World>(this);
	World_->Init();
}

void IGame::CreateWindow(const unsigned int& width, const unsigned int& height, const std::string& title)
{
	Window_ = std::make_unique<Window>(this, width, height, title);
	Window_->Init();
}

void IGame::InitInput()
{
	Input_ = std::make_unique<Input>(this, GetWindow());
	Input_->Init();
}

void IGame::InitRenderer()
{
	Renderer_ = std::make_unique<Renderer>(this);
	Renderer_->Init();
}
