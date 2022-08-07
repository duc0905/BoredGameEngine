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
	std::shared_ptr<Context> next_;
	// Usinged Not match -> int
	int priority_;
	bool isActive_;

	std::map<std::pair<KeyInput::Key, int>, std::string> actionMap_;
	std::map<std::pair<KeyInput::Key, int>, std::pair<std::string, float>> rangeMap_;

	void ResetPriority(int priority);
	void Activate();
	void Deactivate();
public:
	Context() = default;

	void Debuggin();
	
	void AddActionMapping(KeyInput::Key key, int mods, const std::string& name);
	void RemoveActionMapping(KeyInput::Key key, int mods);

	void AddRangeMapping(KeyInput::Key key, int mods, const std::string& name, const float& weight);
	void RemoveRangeMapping(KeyInput::Key key, int mods);

	std::string MapKeyAction(KeyInput::Key key, int mods);
	std::pair<std::string, float> MapKeyRange(KeyInput::Key key, int mods);
};

