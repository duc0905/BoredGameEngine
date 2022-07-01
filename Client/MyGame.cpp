#include "MyGame.h"

MyGame::MyGame()
{
}

void MyGame::Init()
{
	//CreateWindow(800, 600, "Ourscraft");
	//CreateWorld();
	//InitInput();
	//InitRenderer();
	AddSystem<Window>(this, 800, 600, "Ourscraft");
	AddSystem<World>(this);
	AddSystem<Renderer>(this);
	AddSystem<Input>(this, Get<Window>());
}

void MyGame::Stop()
{
}
