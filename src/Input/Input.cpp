#include "Input.h"

Input::Input(IGame* game, const Window& window)
	: System(game), Window_(window)
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
	//std::cout << "Mouse pos: " << x << " " << y << std::endl;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//std::cout << "Mouse pos: " << x << " " << y << std::endl;
	std::cout << (action == GLFW_PRESS ? "Press " : "Release ");
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		std::cout << "left mouse" << std::endl;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		std::cout << "right mouse" << std::endl;
		break;
	}
}

void Input::MouseEnterCallback(GLFWwindow* window, int entered)
{
	//std::cout << "Mouse : " << (entered ? "Entered" : "Left") << std::endl;
}

void Input::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
}
