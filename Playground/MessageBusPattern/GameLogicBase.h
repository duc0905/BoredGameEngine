#pragma once
#include <memory>
#include "Module.h"
#include "Message.h"
#include "MessageBus.h"

class MessageBus;
class Module;

class GameLogicBase
{
private:
	std::shared_ptr<MessageBus> mb_;
	bool isRunning_ = true;

	virtual void OnMessage(Message& msg) = 0;
	virtual void OnUpdate(double dt) = 0;
	virtual void OnDelete() = 0;
	virtual void OnInit() = 0;
protected:
	virtual void Loop() final;
	virtual void SendMessage(Message& msg) final;
	virtual void UseModule(Module* mod) final;
	virtual void RemoveModule(Module* mod) final;
public:
	GameLogicBase();
	virtual void Run() final;

	~GameLogicBase();
};