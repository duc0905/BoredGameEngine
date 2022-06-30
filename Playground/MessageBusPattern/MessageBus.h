#pragma once
#include <vector>
#include <string>
#include <memory>

#include "GameLogicBase.h"
#include "Module.h"
#include "Message.h"
#include "LogMessage.h"

class GameLogicBase;
class MessageBus
{
	friend class Module;
private:
	std::weak_ptr<GameLogicBase> gameLogic_;
	std::vector<std::unique_ptr<Module>> modules_;
protected:
	std::vector<std::unique_ptr<Module>>::iterator FindModule(Module* mod);
public:
	MessageBus() = default;
	MessageBus(std::shared_ptr<GameLogicBase> gameLogic)
	: gameLogic_(std::weak_ptr<GameLogicBase>(gameLogic)), 
		modules_(std::vector<std::unique_ptr<Module>>(0)) {};

	void AddModule(std::unique_ptr<Module> mod);
	void RemoveModule(Module* mod);

	void SendMessage(Message& msg);

	void Delete();

	~MessageBus();
};

