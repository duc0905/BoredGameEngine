#include "../pch.h"
#include "Actor.h"

std::shared_ptr<Actor> Actor::NullActor = std::make_shared<Actor>(0);

unsigned int Actor::IDCounter_ = 1;

Actor::Actor()
{
	ID = IDCounter_;
	IDCounter_++;
	CreateComponent<TransformComponent>();
}

Actor::Actor(unsigned int assignID)
{
	ID = assignID;
}
