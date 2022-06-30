#include "ActorJumpCommand.h"

ActorJumpCommand::ActorJumpCommand(Actor& actor) : _actor(actor)
{
}

void ActorJumpCommand::execute()
{
	std::cout << "Actor Jumped" << std::endl;
	_actor.MoveTo(_actor.GetX(), _actor.GetY() + 1);
}

void ActorJumpCommand::undo()
{
	std::cout << "Actor Jumped undo" << std::endl;
	_actor.MoveTo(_actor.GetX(), _actor.GetY() - 1);
}
