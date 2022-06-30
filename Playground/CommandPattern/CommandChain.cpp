#include "CommandChain.h"

CommandChain::CommandChain()
	: _len(0), _head(nullptr), _tail(nullptr), _curr(nullptr)
{
}

void CommandChain::DeleteFrom(std::shared_ptr<ChainNode> from)
{
	auto _ptr = from->next;
	from->prev = nullptr;

	while (_ptr) {
		auto _temp_ptr = _ptr->next;
		_ptr.reset();
		_ptr = _temp_ptr;
	}
}

void CommandChain::PushCommand(std::shared_ptr<Command> cmd)
{
	if (_curr->next) 
	{
		DeleteFrom(_curr->next);
	}

	auto newNode = std::make_shared<ChainNode>(cmd, _tail);

	if (_head) {
		_tail->next = newNode;
		_tail = newNode;
	}
	else {
		_head = _tail = newNode;
	}

	_curr = newNode;
	_len++;
}

void CommandChain::Undo()
{
	_curr->cmd->undo();
	_curr = _curr->prev;
}

void CommandChain::Redo()
{
	_curr->next->cmd->execute();
	_curr = _curr->next;
}
