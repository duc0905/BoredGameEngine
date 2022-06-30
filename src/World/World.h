#pragma once

#include "../System.h"

class World : public System
{
public:
	World(IGame* game);
	void Update(float dt) {}

	// Inherited via System
	virtual void Init() override;
};

