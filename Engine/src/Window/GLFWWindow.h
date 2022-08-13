#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "IWindow.h"

class GLFWWindow : public IWindow
{
	GLFWwindow* Window_;
	unsigned int Width_, Height_;
	std::string Title_;
public:
	GLFWWindow(const unsigned int& width, const unsigned int& height, const std::string& title = "Bored game");
	~GLFWWindow();

	// Inherited via System
	virtual void Init() override;

	// Inherited via IWindow
	virtual unsigned int GetWidth() const override { return Width_; }
	virtual unsigned int GetHeight() const override { return Height_; }
	virtual bool ShouldClose() const override { return glfwWindowShouldClose(Window_); }
	virtual const std::string& GetTitle() const override { return Title_; };

	inline GLFWwindow* GetGLFWWindow() const { return Window_; }
	inline void SwapBuffer() { glfwSwapBuffers(Window_); }

	// Inherited via IWindow
	virtual void OnTick(float) override;
};

