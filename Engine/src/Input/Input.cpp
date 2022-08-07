#include "Input.h"
#include <vector>
std::shared_ptr<Input> Input::instance_ = nullptr;

Input::Input(const Window& window)
	: Window_(window)
{
	SetupCallbacks();
}

std::shared_ptr<Input> Input::GetInstance() {
	if (!instance_) {
		instance_ = std::shared_ptr<Input>(new Input(IGame::GetWindow()));
	}
	return instance_;
}

void Input::EvaluateKey(KeyInput::Key key, KeyInput::Action action, int mods)
{
	std::string actionName = headContext->MapKeyAction(key, mods);
	std::cout << actionName << std::endl;
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
	AddContext(std::shared_ptr<Context>(con));
}

void Input::AddContext(std::shared_ptr<Context> con)
{
	if (headContext) {
		auto cur = headContext;
		while (cur->next_) {
			cur = cur->next_;
		}
		cur->next_ = con;
		con->ResetPriority(cur->priority_ + 1);
	}
	else {
		headContext = con;
	}
}

void Input::RemoveContext(Context* con)
{
	RemoveContext(std::shared_ptr<Context>(con));
}

void Input::RemoveContext(std::shared_ptr<Context> con)
{
	auto cur = headContext;
	if (cur) {
			while (cur->next_) {
				if (cur->next_ == con) {
					cur->next_ = cur->next_->next_;
					break;
				}
				cur = cur->next_;
			}
			if(headContext == con)
				headContext.reset();
	}
}

void Input::ActivateContext(Context* con)
{
	ActivateContext(std::shared_ptr<Context> (con));
}

void Input::ActivateContext(std::shared_ptr<Context> con)
{
	con->Activate();
}

void Input::DeactivateContext(Context* con)
{
	DeactivateContext(std::shared_ptr<Context>(con));
}

void Input::DeactivateContext(std::shared_ptr<Context> con)
{
	con->Deactivate();
}
void Input::ResetPriority(Context* con, int priorityLevel) {
	ResetPriority(std::shared_ptr<Context>(con), priorityLevel);
}

void Input::ResetPriority(std::shared_ptr<Context> con, int priorityLevel)
{	
	auto cur = headContext;
	std::shared_ptr<Context> target;
	std::shared_ptr<Context> nextLargerContext;
	if (cur) {
		while (cur->next_ && (!target && !nextLargerContext)) {
			if (cur->priority_ <= priorityLevel) {
				nextLargerContext = cur;
			}
			if (cur->next_ == con) {
				target = cur->next_;
				cur->next_ = cur->next_->next_;
			}
			cur = cur->next_;
		}
		if(target){
			target->next_ = nextLargerContext->next_;
			nextLargerContext->next_ = target;
		}
	}
}

KeyInput::Key Input::GetKey(int keyCode)
{
	switch (keyCode) {
	case GLFW_KEY_1: return KeyInput::KEY_1;
	case GLFW_KEY_2: return KeyInput::KEY_2;
	case GLFW_KEY_3: return KeyInput::KEY_3;
	case GLFW_KEY_4: return KeyInput::KEY_4;
	case GLFW_KEY_5: return KeyInput::KEY_5;
	case GLFW_KEY_6: return KeyInput::KEY_6;
	case GLFW_KEY_7: return KeyInput::KEY_7;
	case GLFW_KEY_8: return KeyInput::KEY_8;
	case GLFW_KEY_9: return KeyInput::KEY_9;
	case GLFW_KEY_0: return KeyInput::KEY_0;
	case GLFW_KEY_Q: return KeyInput::KEY_Q; 
	case GLFW_KEY_W: return KeyInput::KEY_W;
	case GLFW_KEY_E: return KeyInput::KEY_E;
	case GLFW_KEY_R: return KeyInput::KEY_R;
	case GLFW_KEY_T: return KeyInput::KEY_T;
	case GLFW_KEY_Y: return KeyInput::KEY_Y;
	case GLFW_KEY_U: return KeyInput::KEY_U;
	case GLFW_KEY_I: return KeyInput::KEY_I;
	case GLFW_KEY_O: return KeyInput::KEY_O; 
	case GLFW_KEY_P: return KeyInput::KEY_P;
	case GLFW_KEY_A: return KeyInput::KEY_A;
	case GLFW_KEY_S: return KeyInput::KEY_S;
	case GLFW_KEY_D: return KeyInput::KEY_D;
	case GLFW_KEY_F: return KeyInput::KEY_F;
	case GLFW_KEY_G: return KeyInput::KEY_G;
	case GLFW_KEY_H: return KeyInput::KEY_H;
	case GLFW_KEY_J: return KeyInput::KEY_J;
	case GLFW_KEY_K: return KeyInput::KEY_K;
	case GLFW_KEY_L: return KeyInput::KEY_L;
	case GLFW_KEY_Z: return KeyInput::KEY_Z;
	case GLFW_KEY_X: return KeyInput::KEY_X;
	case GLFW_KEY_C: return KeyInput::KEY_C;
	case GLFW_KEY_V: return KeyInput::KEY_V;
	case GLFW_KEY_B: return KeyInput::KEY_B; 
	case GLFW_KEY_N: return KeyInput::KEY_N;
	case GLFW_KEY_M: return KeyInput::KEY_M;
	default: return KeyInput::KEY_UNKNOWN;
	}
}

int Input::GetMods(int modBits)
{
	int ans = 0;
	if (modBits & GLFW_MOD_CONTROL) 
		ans |= KeyInput::MOD_CTRL;
	if (modBits & GLFW_MOD_SHIFT)
		ans |= KeyInput::MOD_SHIFT;
	if (modBits & GLFW_MOD_ALT)
		ans |= KeyInput::MOD_ALT;
	return ans;
}

KeyInput::Action Input::GetAction(int actionCode)
{
	switch (actionCode) {
	case GLFW_PRESS: return KeyInput::PRESS;
	case GLFW_REPEAT: return KeyInput::REPEAT;
	case GLFW_RELEASE: return KeyInput::RELEASE;
	default: return KeyInput::UNKNOWN;
	}
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
	auto input = GetInstance();
	input->EvaluateKey(GetKey(key),GetAction(action), GetMods(mods));
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
