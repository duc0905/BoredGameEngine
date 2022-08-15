#include "GLFWWindow.h"

GLFWWindow::GLFWWindow(const unsigned int& width, const unsigned int& height, const std::string& title)
    : Width_(width), Height_(height), Title_(title)
{
}

GLFWWindow::~GLFWWindow()
{
    std::cout << "Cleaning up GLFWWindow!" << std::endl;
    glfwTerminate();
}

void GLFWWindow::Init()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        glfwTerminate();
        std::cout << "Cannot initialize glfw" << std::endl;
    }

    /* Create a windowed mode window and its OpenGL context */
    Window_ = glfwCreateWindow(Width_, Height_, Title_.c_str(), NULL, NULL);
    if (!Window_)
    {
        glfwTerminate();
        std::cout << "Cannot create window" << std::endl;
    }
    else {
        std::cout << "Created window" << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(Window_);
}

void GLFWWindow::OnTick(float)
{
    SwapBuffer();
}
