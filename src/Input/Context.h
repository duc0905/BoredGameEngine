#pragma once
#include <iostream>
#include <map>
#include <string>

namespace KeyInput
{
	enum Key
	{};

	enum Mod
	{};

	enum Action
	{};
}

enum ActionEnum
{
	ACTION,
	RANGE
};

class Input;

/* Context
* Define a set of actions/state/ranges those are available when this context is active
* Multiple contexts can be active at once
* Using Chain of responsibility pattern
*/


class Context
{
	friend class Input;

private:
	Context* next_;
	unsigned int priority_;
	bool isActive_;

	std::map<std::pair<KeyInput::Key, KeyInput::Mod>, std::string> actionMap_;
	std::map<std::pair<KeyInput::Key, KeyInput::Mod>, std::pair<std::string, float>> rangeMap_;

	void ResetPriority(int priority);
	void Activate();
	void Deactivate();
public:
	Context() = default;
	
	void AddActionMapping(KeyInput::Key key, KeyInput::Mod mods, const std::string& name);
	void RemoveActionMapping(KeyInput::Key key, KeyInput::Mod mods);

	void AddRangeMapping(KeyInput::Key key, KeyInput::Mod mods, const std::string& name, const float& weight);
	void RemoveRangeMapping(KeyInput::Key key, KeyInput::Mod mods);

	std::pair<ActionEnum, std::string> MapKey(KeyInput::Key key, KeyInput::Mod mods, KeyInput::Action);
};

