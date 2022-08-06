#pragma once
#include <iostream>
#include <map>
#include <string>
#include "Constant.h"

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

	void Debuggin() {
		std::cout << "Printing Action Map" << std::endl;
		for (const auto& elem : actionMap_) {
			std::cout << elem.first.first << " " << elem.first.second << " " << elem.second << std::endl;
		};

		std::cout << "Printing Range Map" << std::endl;
		for (const auto& elem : rangeMap_) {
			std::cout << elem.first.first << " " << elem.first.second << " " << elem.second.first << " " << elem.second.second << std::endl;
		};

	}
	
	void AddActionMapping(KeyInput::Key key, KeyInput::Mod mods, const std::string& name);
	void RemoveActionMapping(KeyInput::Key key, KeyInput::Mod mods);

	void AddRangeMapping(KeyInput::Key key, KeyInput::Mod mods, const std::string& name, const float& weight);
	void RemoveRangeMapping(KeyInput::Key key, KeyInput::Mod mods);

	std::pair<ActionEnum, std::string> MapKey(KeyInput::Key key, KeyInput::Mod mods, KeyInput::Action);
};

