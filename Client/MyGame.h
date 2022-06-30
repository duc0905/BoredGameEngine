#pragma once
#include "../src/IGame.h"

class MyGame : public IGame
{
	// Inherited via IGame
	virtual void Init() override;
	virtual void Stop() override;
public:
	MyGame();
};

