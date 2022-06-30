#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../System.h"

class Window : public System
{
	GLFWwindow* Window_;
	unsigned int Width_, Height_;
	std::string Title_;
public:
	Window(IGame* game, const unsigned int& width, const unsigned int& height, const std::string& title = "My game");
	~Window();

	inline const unsigned int GetWidth() const { return Width_; }
	inline const unsigned int GetHeight() const { return Height_; }
	inline GLFWwindow* GetGLFWWindow() const { return Window_; }

	inline bool ShouldClose() { return glfwWindowShouldClose(Window_); }
	inline void SwapBuffer() { glfwSwapBuffers(Window_); }

	// Inherited via System
	virtual void Init() override;
};

