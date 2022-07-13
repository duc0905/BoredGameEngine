#pragma once

#include <set>
#include "../Actor/Actor.h"
#include "../System.h"

class World : public System
{
private:
	std::set<std::shared_ptr<Actor>> actors_;
public:
	World();
	void Update(float dt);

	// Inherited via System
	virtual void Init() override;

	virtual ~World() override
	{
		std::cout << "Cleaning up World!" << std::endl;
	}

	std::set<std::shared_ptr<Actor>> GetActors() const
	{
		return actors_;
	}
	void AddActor(std::shared_ptr<Actor> actor);
	void RemoveActor(std::shared_ptr<Actor> actor);
};

