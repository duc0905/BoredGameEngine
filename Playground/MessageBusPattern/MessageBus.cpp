#include "MessageBus.h"
#include <iostream>

void MessageBus::SendMessage(Message& msg)
{
	for (auto& mod : modules_) {
		mod->OnMessage(msg);
	}
}

MessageBus::~MessageBus()
{
	Delete();
}

std::vector<std::unique_ptr<Module>>::iterator MessageBus::FindModule(Module* mod)
{
	std::vector<std::unique_ptr<Module>>::iterator it = modules_.begin();

	for (; it != modules_.end(); it++)
		if (it->get() == mod)
			break;

	return it;
}

void MessageBus::AddModule(std::unique_ptr<Module> mod)
{
	if (std::find(modules_.begin(), modules_.end(), mod) == modules_.end())
	{
		modules_.push_back(std::move(mod));
	}
	else {
		LogMessage msg("Module has already existed", LogSeverity::WARNING);
		SendMessage(msg);
	}
}

void MessageBus::RemoveModule(Module* mod)
{
	auto pos = FindModule(mod);
	if (pos == modules_.end())
	{
		// Send a warning log
		LogMessage msg("Module not registered", LogSeverity::WARNING);
	}
	else
		modules_.erase(pos);
}

void MessageBus::Delete()
{
	for (auto& mod : modules_) {
		mod.release();
	}
}
