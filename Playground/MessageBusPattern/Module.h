#pragma once
#include <memory>
//#include "Message.h"
#include "MessageBus.h"

class Message;
class Module
{
	friend class MessageBus;
private:
	std::weak_ptr<MessageBus> mb_;
	virtual void AttachMessageBus(std::shared_ptr<MessageBus> MessageBus) final;
	virtual void OnAttach() {};

	Module() = default;

	virtual void OnMessage(Message& msg) = 0;
	virtual void OnUpdate(double dt) {};
protected:
	virtual void SendMessage(Message& msg) final;

	std::shared_ptr<MessageBus> GetMessageBus() const { return mb_.lock(); }
};
