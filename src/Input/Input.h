#pragma once
#include <iostream>
#include <array>
#include "../System.h"
#include "../Window/Window.h"

class Input : public System
{
private:
	const Window& Window_;

	void SetupCallbacks();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseEnterCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double x, double y);
public:
	Input(const Window& window);
	static void PollEvents();

	// Inherited via System
	virtual void Init() override;

	bool isKeyDown(const char& c)
	{
		if (!std::isalnum(c))
			return false;
		return keyDown_[(int)(std::tolower(c))];
	}

	virtual ~Input() override
	{
		std::cout << "Cleaning up Input!" << std::endl;
	}
private:
	static std::array<bool, 256> keyDown_;
	static double mouseX_, mouseY_;
	static bool leftMouseDown_, rightMouseDown_, middleMouseDown_;
};

