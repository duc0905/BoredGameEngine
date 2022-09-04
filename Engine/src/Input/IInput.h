#pragma once
#include "../pch.h"

#include "../System.h"
#include "Context.h"

class IInput : public System
{
public:
	typedef std::function<void(KeyInput::Action)> ActionCallback;
	typedef std::function<void(KeyInput::Action, float)> RangeCallback;
private:
	static std::shared_ptr<IInput> defaultInput_;

	std::shared_ptr<Context> headContext;

	std::map<std::string, ActionCallback> actionMap;
	std::map<std::string, std::pair<RangeCallback, float>> rangeMap;

protected:
	void EvaluateKey(KeyInput::Key key, KeyInput::Action action, int mods, double val);
	//void EvaluateKey(KeyInput::Key key, int mods, double val);
	
public:
	virtual ~IInput() { };
	static std::shared_ptr<IInput> GetDefault() { return defaultInput_; }

	void BindAction(const std::string& name, ActionCallback func);
	void BindRange(const std::string& name, RangeCallback func, float val = 1.0f);

	void AddContext(Context* con);
	void AddContext(std::shared_ptr<Context> con);

	void RemoveContext(Context* con);
	void RemoveContext(std::shared_ptr<Context> con);

	void ActivateContext(Context* con);
	void ActivateContext(std::shared_ptr<Context> con);

	void DeactivateContext(Context* con);
	void DeactivateContext(std::shared_ptr<Context> con);

	void ResetPriority(Context* con, int priorityLevel);
	void ResetPriority(std::shared_ptr<Context> con, int priorityLevel);

	virtual KeyInput::Key GetKey(int keyCode) = 0;
	virtual int GetMods(int modBits) = 0;
	virtual KeyInput::Action GetAction(int actionCode) = 0;
};

class NullInput : public IInput
{
	virtual KeyInput::Key GetKey(int keyCode) { return KeyInput::KEY_UNKNOWN; };
	virtual int GetMods(int modBits) { return KeyInput::MOD_UNKNOWN; };
	virtual KeyInput::Action GetAction(int actionCode) { return KeyInput::UNKNOWN; };
};
