#include "GameLogicBase.h"

GameLogicBase::GameLogicBase()
{
	mb_ = std::make_shared<MessageBus>();
}

void GameLogicBase::Run()
{
	OnInit();
	Loop();
	OnDelete();
}

void GameLogicBase::Loop()
{
	while (isRunning_) {
		OnUpdate(0.01f);
		auto inputMsg = Message(MessageType::MODULE_INPUT_UPDATE);
		SendMessage(inputMsg);
	}
}

void GameLogicBase::SendMessage(Message& msg)
{
	mb_->SendMessage(msg);
}

void GameLogicBase::UseModule(Module* mod)
{
	mb_->AddModule(std::unique_ptr<Module>(mod));
}

void GameLogicBase::RemoveModule(Module* mod)
{
	mb_->RemoveModule(mod);
}

GameLogicBase::~GameLogicBase()
{
	mb_.reset();
}
