#include "World.h"

World::World()
{
}

void World::Init()
{
}

void World::Update(float dt)
{
	for (auto actor : actors_) {
		actor->OnUpdate(dt);
	}
}

void World::AddActor(std::shared_ptr<Actor> actor)
{
	// Actor already been added
	if (actors_.find(actor) != actors_.end())
	{
		std::cout << "Warning: Actor has already been added to the world!" << std::endl;
		return;
	}

	actors_.insert(actor);
	actor->OnAttach();
}

void World::RemoveActor(std::shared_ptr<Actor> actor)
{
	if (actors_.find(actor) == actors_.end())
	{
		std::cout << "Warning: Actor has not been added to the world!" << std::endl;
		return;
	}

	actors_.erase(actor);
	actor->OnDetach();
}
