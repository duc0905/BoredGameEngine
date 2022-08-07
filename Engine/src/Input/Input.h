#pragma once
#include <iostream>
#include <array>
#include <map>
#include <string>
#include <functional>

#include "Context.h"
#include "../System.h"
#include "../Window/Window.h"
#include "../IGame.h"

class Input : public System
{
public:
	typedef std::function<void(KeyInput::Action)> ActionCallback;
	typedef std::function<void(KeyInput::Action, float)> RangeCallback;

private:
	const Window& Window_;
	static std::shared_ptr<Input> instance_;

	Input(const Window& window);

	std::shared_ptr<Context> headContext;

	void SetupCallbacks();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseEnterCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double x, double y);

	std::map<std::string, ActionCallback> actionMap;
	std::map<std::string, std::pair<RangeCallback, float>> rangeMap;

public:

	void PollEvents();

	// Inherited via System
	virtual void Init() override;

	static std::shared_ptr<Input> GetInstance();

	void EvaluateKey(KeyInput::Key key, KeyInput::Action action, int mods);

	void BindAction(const std::string& name, ActionCallback func);
	void BindRange(const std::string& name, RangeCallback func, float weight = 1.0f);
	
	void AddContext(Context* con);
	void AddContext(std::shared_ptr<Context> con);
	void RemoveContext(Context* con);
	void RemoveContext(std::shared_ptr<Context> con);
	
	void ActivateContext(Context* con);
	void ActivateContext(std::shared_ptr<Context> con);
	void DeactivateContext(Context* con);
	void DeactivateContext(std::shared_ptr<Context> con);
	
	void ResetPriority(Context* con, int priorityLevel = 10000);
	void ResetPriority(std::shared_ptr<Context> con, int priorityLevel = 10000);
	
	static KeyInput::Key GetKey(int keyCode);
	static int GetMods(int modBits);
	static KeyInput::Action GetAction(int actionCode);

	virtual ~Input() override
	{
		std::cout << "Cleaning up Input!" << std::endl;
	}
};

