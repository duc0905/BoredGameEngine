#include "MyGame.h"

MyGame::MyGame()
{
}

void MyGame::Init()
{
	CreateWindow(800, 600, "Ourscraft");
	CreateWorld();
	InitInput();
	InitRenderer();
}

void MyGame::Stop()
{
	DeleteWindow();
	DeleteWorld();
}
