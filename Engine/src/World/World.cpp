#include "pch.h"
#include "World.h"

World::World()
{
}

void World::Init()
{
}

void World::OnTick(float dt)
{
	// Tick the gamemode
	if (gm_)
		gm_->OnTick(dt);

	// Tick the actors
	for (auto actor : GetActors())
		actor->OnTick(dt);
}

Actor& World::GetActor(unsigned int actorID)
{
	for (auto actor : actors_) 
		if (actor->GetID() == actorID)
			return *actor;
	return Actor::NullActor;
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
