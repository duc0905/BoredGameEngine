#include "Input.h"
#include <vector>

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
	SetupCallbacks();
}

void Input::BindAction(const std::string& name, ActionCallback)
{
}

void Input::BindRange(const std::string& name, RangeCallback)
{
}

void Input::RemoveContext(Context* con)
{
}

void Input::ActivateContext(Context* con)
{
}

void Input::DeactivateContext(Context* con)
{
}

void Input::ResetPriority(Context* con, int priorityLevel = 10000)
{
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
}

void Input::MousePosCallback(GLFWwindow* window, double x, double y)
{
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void Input::MouseEnterCallback(GLFWwindow* window, int entered)
{
}

void Input::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
}
