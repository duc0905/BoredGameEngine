#include "../pch.h"
#include "Actor.h"

unsigned int Actor::IDCounter_ = 0;

Actor Actor::NullActor = Actor();
Actor::Actor()
{
	ID = IDCounter_;
	IDCounter_++;
	CreateComponent<TransformComponent>();
}

