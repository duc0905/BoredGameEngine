#include "Input.h"
#include <vector>

double Input::mouseX_ = 0.0f;
double Input::mouseY_ = 0.0f;
bool Input::leftMouseDown_ = false;
bool Input::middleMouseDown_ = false;
bool Input::rightMouseDown_ = false;
std::array<bool, 256> Input::keyDown_ = {false};

Input::Input(const Window& window)
	: Window_(window)
{
}

void Input::PollEvents()
{
	glfwPollEvents();
}

void Input::Init()
{
	keyDown_.fill(false);
	SetupCallbacks();
}

void Input::SetupCallbacks()
{
	auto window = Window_.GetGLFWWindow();
	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetCursorPosCallback(window, Input::MousePosCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetCursorEnterCallback(window, Input::MouseEnterCallback);
	glfwSetScrollCallback(window, Input::MouseScrollCallback);
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (GLFW_KEY_A <= key && key <= GLFW_KEY_Z)
	{
		int mappedKeyPos = key - GLFW_KEY_A + (int)('a');
		if (action == GLFW_PRESS)
			keyDown_[mappedKeyPos] = true;
		else if (action == GLFW_RELEASE)
			keyDown_[mappedKeyPos] = false;
	}

	if (GLFW_KEY_0 <= key && key <= GLFW_KEY_9)
	{
		int mappedKeyPos = key - GLFW_KEY_0 + (int)('0');
		if (action == GLFW_PRESS)
			keyDown_[mappedKeyPos] = true;
		else if (action == GLFW_RELEASE)
			keyDown_[mappedKeyPos] = false;
	}
	// Broadcast to listeners
}

void Input::MousePosCallback(GLFWwindow* window, double x, double y)
{
	mouseX_ = x;
	mouseY_ = y;
	// Broadcast to listeners
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Broadcast to listeners
}

void Input::MouseEnterCallback(GLFWwindow* window, int entered)
{
	// Broadcast to listeners
}

void Input::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	// Broadcast to listeners
}
