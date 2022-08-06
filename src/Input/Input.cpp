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

void Input::BindAction(const std::string& name, ActionCallback func)
{
	auto it = actionMap.find(name);
	if (it != actionMap.end()) {
		std::cout << "WARNING! Overwriting alias for action function" << std::endl;
	};
	actionMap[name] = func;
}

void Input::BindRange(const std::string& name, RangeCallback func)
{
	auto it = rangeMap.find(name);
	if (it != rangeMap.end()) {
		std::cout << "WARNING! Overwriting alias for range function" << std::endl;
	};
	rangeMap[name] = func;
}

void Input::AddContext(Context* con)
{
	if (headContext != nullptr) {
		Context* cur = headContext.get();
		while (cur->next_ != NULL) {
			cur = cur->next_;
		}
		cur->next_ = con;
		con->ResetPriority(cur->priority_ + 1);
		delete cur;
	}
	else {
		std::shared_ptr<Context> new_head(con);
		headContext = new_head;
	}
}

void Input::RemoveContext(Context* con)
{
	Context* cur = headContext.get();
	if (cur != NULL) {
		if ((cur->next_) != NULL) {
			while ((cur->next_->next_) != NULL) {
				cur = cur->next_;
			}
			cur->next_ = NULL;
		}
		headContext = nullptr;
	}
	delete cur;
}

void Input::ActivateContext(Context* con)
{
	Context* cur = headContext.get();
	while (cur != NULL) {
		if (cur == con) {
			con->Activate();
		}
		cur = cur->next_;
	}
	delete cur;
}

void Input::DeactivateContext(Context* con)
{
	Context* cur = headContext.get();
	while (cur != NULL) {
		if (cur == con) {
			con->Deactivate();
		}
		cur = cur->next_;
	}
	delete cur;
}

void Input::ResetPriority(Context* con, int priorityLevel)
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
