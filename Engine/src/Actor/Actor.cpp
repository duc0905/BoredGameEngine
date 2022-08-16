#include "../pch.h"
#include "Actor.h"

Actor::Actor()
{
	CreateComponent<TransformComponent>();
}
