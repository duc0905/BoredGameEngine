#include "MyGame.h"

MyGame::MyGame()
{
}

void MyGame::Init()
{
	AddSystem<Window>( 800, 600, "Ourscraft");
	AddSystem<World>();
	AddSystem<Renderer>();
	AddSystem<Input>(*Get<Window>());
}

void MyGame::Stop()
{
}
