#pragma once
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

/* Context
* Define a set of actions/state/ranges those are available when this context is active
* Multiple contexts can be active at once
* Using Chain of responsibility pattern
*/
class Context
{
	Context* next;
	std::map<std::pair<KeyInput::Key, KeyInput::Mod>, std::pair<ActionEnum, std::string>> keyMap_;
public:
	Context() = default;

	void SetActionKeyMapping(KeyInput::Key key, KeyInput::Mod mods, std::string name);
	void RemoveActionKeyMapping(std::string name);

	void Activate();
	void Deactivate();
	std::pair<ActionEnum, std::string> MapKey(KeyInput::Key key, KeyInput::Mod mods, KeyInput::Action );
};

