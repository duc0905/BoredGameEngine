#pragma once
#include "../pch.h"
#include "IWorld.h"
#include "../Actor/Actor.h"

class World : public IWorld
{
private:
	// 2 actor with the same pointer address is not allowed
	std::set<std::shared_ptr<Actor>> actors_;

public:
	World();

	virtual ~World() override
	{
		std::cout << "Cleaning up World!" << std::endl;
	}

	// Inherited via System
	virtual void Init() override;
	virtual void OnTick(float dt) override;


	virtual std::vector<std::shared_ptr<Actor>> GetActors() override
	{
		return std::vector<std::shared_ptr<Actor>>(actors_.begin(), actors_.end());
	}
	virtual void AddActor(std::shared_ptr<Actor> actor) override;
	virtual void RemoveActor(std::shared_ptr<Actor> actor) override;
};

