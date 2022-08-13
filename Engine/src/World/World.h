#pragma once

#include <set>
#include "../Actor/Actor.h"
#include "IWorld.h"

class World : public IWorld
{
private:
	std::set<std::shared_ptr<Actor>> actors_;
public:
	World();

	// Inherited via System
	virtual void Init() override;

	virtual void OnTick(float dt) override;

	virtual ~World() override
	{
		std::cout << "Cleaning up World!" << std::endl;
	}

	virtual std::vector<std::shared_ptr<Actor>> GetActors() override
	{
		return std::vector<std::shared_ptr<Actor>>(actors_.begin(), actors_.end());
	}
	virtual void AddActor(std::shared_ptr<Actor> actor) override;
	virtual void RemoveActor(std::shared_ptr<Actor> actor) override;
};

