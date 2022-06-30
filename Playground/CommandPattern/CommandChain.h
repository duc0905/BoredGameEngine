#pragma once
#include "Command.h"
#include <memory>

struct ChainNode
{
	ChainNode(std::shared_ptr<Command> _cmd, std::shared_ptr<ChainNode> _prev)
		: cmd(_cmd)
	std::shared_ptr<Command> cmd;

	std::shared_ptr<ChainNode> prev;
	std::shared_ptr<ChainNode> next = nullptr;
};

class CommandChain
{
private:
	std::shared_ptr<ChainNode> _head, _tail, _curr;
	unsigned int _len;

	void DeleteFrom(std::shared_ptr<ChainNode>);
public:
	CommandChain();
	void PushCommand(std::shared_ptr<Command> cmd);
	void Undo();
	void Redo();
};

