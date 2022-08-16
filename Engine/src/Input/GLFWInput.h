#pragma once
#include "../pch.h"

#include "../IGame.h"
#include "IInput.h"
#include "../Window/GLFWWindow.h"

class GLFWInput : public IInput
{
private:
	IWindow& window_;
	static std::shared_ptr<GLFWInput> instance_;

	GLFWInput();

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseEnterCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double x, double y);

	void SetupCallbacks();
public:
	virtual void OnTick(float) override;

	static GLFWInput& GetInstance();
	static std::shared_ptr<GLFWInput> GetInstancePtr();

	virtual KeyInput::Key GetKey(int keyCode) override;
	virtual int GetMods(int modBits) override;
	virtual KeyInput::Action GetAction(int actionCode) override;

	virtual ~GLFWInput() override
	{
		//std::cout << "Cleaning up Input!" << std::endl;
		instance_.reset();
	}
};

