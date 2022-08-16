#include "../pch.h"
#include "GLFWWindow.h"

GLFWWindow::GLFWWindow(const unsigned int& width, const unsigned int& height, const std::string& title)
    : Width_(width), Height_(height), Title_(title)
{
}

GLFWWindow::~GLFWWindow()
{
    LOG_COLOR("Cleaning up GLFWWindow!", COLOR::GREEN, COLOR::BLACK);
    glfwTerminate();
}

void GLFWWindow::Init()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        LOG_COLOR("Cannot initialize glfw", COLOR::RED, COLOR::BLACK);
        glfwTerminate();
    }

    /* Create a windowed mode window and its OpenGL context */
    Window_ = glfwCreateWindow(Width_, Height_, Title_.c_str(), NULL, NULL);
    if (!Window_)
    {
        LOG_COLOR("Cannot create window", COLOR::RED, COLOR::BLACK);
        glfwTerminate();
    }
    else {
        LOG_COLOR("Created window", COLOR::BRIGHT_BLUE, COLOR::BLACK);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(Window_);
}

void GLFWWindow::OnTick(float)
{
    SwapBuffer();
}
