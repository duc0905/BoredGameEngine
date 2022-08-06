#include "MyGame.h"

MyGame::MyGame()
{
}

void MyGame::Init()
{
	Window* window = new Window(800, 600, "Ourscraft");
	window->Init();
	SetWindow(window);
	//AddSystem<Window>( 800, 600, "Ourscraft");

	World* world = new World();
	world->Init();
	SetWorld(world);
	//AddSystem<World>();

	Renderer* renderer = new Renderer();
	renderer->Init();
	SetRenderer(renderer);
	//AddSystem<Renderer>();

	Input* input = new Input(*window);
	input->Init();
	SetInput(input);
	//AddSystem<Input>(*Get<Window>());
}

void MyGame::Stop()
{
}
