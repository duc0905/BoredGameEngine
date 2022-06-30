#pragma once
#include <iostream>
#include "Command.h"
#include "Actor.h"

class ActorJumpCommand : public Command
{
private:
	Actor& _actor;
public:
	ActorJumpCommand(Actor& actor);

	// Inherited via Command
	virtual void execute() override;
	virtual void undo() override;
};