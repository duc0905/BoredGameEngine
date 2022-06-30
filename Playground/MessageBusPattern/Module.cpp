#include "Module.h"

void Module::AttachMessageBus(std::shared_ptr<MessageBus> mb)
{
	mb_ = std::weak_ptr<MessageBus>(mb);
}

void Module::SendMessage(Message& msg)
{
	GetMessageBus()->SendMessage(msg);
}
