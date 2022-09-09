#include "../pch.h"
#include "GLFWInput.h"

std::shared_ptr<GLFWInput> GLFWInput::instance_ = nullptr;

GLFWInput::GLFWInput()
	: window_(IGame::GetWindow())
{
	if (auto w = std::dynamic_pointer_cast<GLFWWindow>(IGame::GetWindowPtr()))
		SetupCallbacks();
	else
	{
		std::stringstream ss;
		ss << "You are using GLFW input system." << std::endl;
		ss << "GLFW input system requires to use GLFW window to setup input utilities." << std::endl << std::endl;
		LOG_COLOR(ss.str(), COLOR::YELLOW, COLOR::BLACK);

		ss.clear();

		ss << "Your code should looks something like:" << std::endl << std::endl;
		ss << "GLFWWindow* window = new GLFWWindow(800, 600, \"My game\");" << std::endl;
		ss << "IGame::SetWindow(window);" << std::endl;
		ss << "..." << std::endl;

		ss << "IGame::SetInput(input);" << std::endl << std::endl;

		LOG_COLOR(ss.str(), COLOR::BRIGHT_BLUE, COLOR::BLACK);
	}
}

GLFWInput& GLFWInput::GetInstance() {
	if (!instance_) {
		instance_ = std::shared_ptr<GLFWInput>(new GLFWInput());
		instance_->Init();
	}

	return *instance_;
}

std::shared_ptr<GLFWInput> GLFWInput::GetInstancePtr()
{
	if (!instance_)
		GetInstance();
	return instance_;
}

void GLFWInput::OnTick(float)
{
	glfwPollEvents();
}


KeyInput::Key GLFWInput::GetKey(int keyCode)
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
		// mouse
		case GLFW_MOUSE_BUTTON_1: return KeyInput::KEY_MB_1;
		case GLFW_MOUSE_BUTTON_2: return KeyInput::KEY_MB_2;
		case GLFW_MOUSE_BUTTON_3: return KeyInput::KEY_MB_3;
		case GLFW_MOUSE_BUTTON_4: return KeyInput::KEY_MB_4;
		case GLFW_MOUSE_BUTTON_5: return KeyInput::KEY_MB_5;
		case GLFW_MOUSE_BUTTON_6: return KeyInput::KEY_MB_6;
		case GLFW_MOUSE_BUTTON_7: return KeyInput::KEY_MB_7;
		case GLFW_MOUSE_BUTTON_8: return KeyInput::KEY_MB_8;
		
		// default
		default: return KeyInput::KEY_UNKNOWN;
	}
}

int GLFWInput::GetMods(int modBits)
{
	int ans = 0;
	if (modBits & GLFW_MOD_CONTROL) 
		ans |= KeyInput::CTRL;
	if (modBits & GLFW_MOD_SHIFT)
		ans |= KeyInput::SHIFT;
	if (modBits & GLFW_MOD_ALT)
		ans |= KeyInput::ALT;
	return ans;
}

KeyInput::Action GLFWInput::GetAction(int actionCode)
{
	switch (actionCode) {
	case GLFW_PRESS: return KeyInput::PRESS;
	case GLFW_REPEAT: return KeyInput::REPEAT;
	case GLFW_RELEASE: return KeyInput::RELEASE;
	default: return KeyInput::UNKNOWN;
	}
}

void GLFWInput::SetupCallbacks()
{
	GLFWwindow* window = dynamic_cast<GLFWWindow&>(window_).GetGLFWWindow();

	glfwSetKeyCallback(window, GLFWInput::KeyCallback);
	glfwSetCursorPosCallback(window, GLFWInput::MousePosCallback);
	glfwSetMouseButtonCallback(window, GLFWInput::MouseButtonCallback);
	glfwSetCursorEnterCallback(window, GLFWInput::MouseEnterCallback);
	glfwSetScrollCallback(window, GLFWInput::MouseScrollCallback);
}


void GLFWInput::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto& input = GetInstance();
	input.EvaluateKey(input.GetKey(key), input.GetAction(action), input.GetMods(mods), 1.0f);
}

void GLFWInput::MousePosCallback(GLFWwindow* window, double x, double y)
{
	auto& input = GetInstance();
	input.mouseInfo.posX = x;
	input.mouseInfo.posY = y;
	input.EvaluateKey(KeyInput::MOUSE_POS_X, KeyInput::PRESS, 0, x);
	input.EvaluateKey(KeyInput::MOUSE_POS_Y, KeyInput::PRESS, 0, y);
}

void GLFWInput::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	auto& input = GetInstance();
	input.EvaluateKey(input.GetKey(button), input.GetAction(action), input.GetMods(mods), 1.0f);
}

void GLFWInput::MouseEnterCallback(GLFWwindow* window, int entered)
{
	auto& input = GetInstance();
	if (entered) {
		input.EvaluateKey(KeyInput::MOUSE_ENTER, KeyInput::PRESS, 0, 1.0f);
		input.mouseInfo.isEntered = true;
	}
	else {
		input.EvaluateKey(KeyInput::MOUSE_LEAVE, KeyInput::PRESS, 0, 1.0f);
		input.mouseInfo.isEntered = false;
	}
}

void GLFWInput::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	auto& input = GetInstance();
	input.EvaluateKey(KeyInput::MOUSE_SCROLL_X, KeyInput::PRESS, 0, x);
	input.EvaluateKey(KeyInput::MOUSE_SCROLL_Y, KeyInput::PRESS, 0, y);
	input.mouseInfo.scrollX = x;
	input.mouseInfo.scrollY = y;
}
