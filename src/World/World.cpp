#include "World.h"

World::World()
{
}

void World::Init()
{
}

void World::Update(float dt)
{
	auto transComp = actors_.begin()->get()->FindComponent<TransformComponent>();
	auto curRotation = transComp->GetRotation();
	transComp->Rotate(curRotation + glm::vec3(glm::pi<float>() / 180 * dt * 10));
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
