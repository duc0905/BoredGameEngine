#pragma once
#include <iostream>
#include <array>
#include <map>
#include <string>
#include <functional>

#include "Context.h"
#include "../System.h"
#include "../Window/Window.h"

class Input : public System
{
private:
	const Window& Window_;

	std::shared_ptr<Context> headContext;

	void SetupCallbacks();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseEnterCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double x, double y);

	std::map<std::string, std::function<void()>> actionMap;
	std::map<std::string, std::function<void(float)>> rangeMap;

public:
	typedef std::function<void()> ActionCallback;
	typedef std::function<void(float)> RangeCallback;

	Input(const Window& window);
	void PollEvents();

	// Inherited via System
	virtual void Init() override;

	void BindAction(const std::string& name, ActionCallback);
	void BindRange(const std::string& name, RangeCallback);
	void AddContext(Context* con);
	void RemoveContext(Context* con);
	void ActivateContext(Context* con);
	void DeactivateContext(Context* con);
	void ResetPriority(Context* con, int priorityLevel = 10000);

	virtual ~Input() override
	{
		std::cout << "Cleaning up Input!" << std::endl;
	}
};

