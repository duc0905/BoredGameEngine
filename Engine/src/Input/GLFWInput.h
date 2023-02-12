#pragma once
#include "../pch.h"

#include "../IGame.h"
#include "IInput.h"
#include "../Window/GLFWWindow.h"

class GLFWInput : public IInput
{
private:
	IWindow& window_;
	static double mouse_X;
	static double mouse_Y;
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

	virtual void SetCursorImage(unsigned char* image, unsigned int width, unsigned int height);
	virtual void EnableCursor();
	virtual void DisableCursor();
	virtual KeyInput::Key GetKey(int keyCode) override;
	virtual int GetMods(int modBits) override;
	virtual KeyInput::Action GetAction(int actionCode) override;

	virtual ~GLFWInput() override
	{
		instance_.reset();
	}
};

