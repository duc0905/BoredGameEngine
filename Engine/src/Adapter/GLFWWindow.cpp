#include <exception>
#include "Window.h"
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>

namespace Bored
{
namespace GLFW
{
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

Window::Window()
{
}

Window::~Window()
{
}

void Window::SetWidth(int w)
{
    width = w;
}

void Window::SetHeight(int h)
{
    height = h;
}

void Window::SetFullscreen(bool f)
{
    fullscreen = f;
}

void Window::SetTitle(const std::string& n)
{
    name = n;
}

int Window::GetWidth() const
{
    return width;
}

int Window::GetHeight() const
{
    return height;
}

void Window::OnSetup()
{
    if (!glfwInit())
        throw std::exception("Cannot initialize GLFW");

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    int nMonitors = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&nMonitors);

    GLFWmonitor* monitor = NULL;
    if (fullscreen)
        monitor = monitors[0];

    nativeWindow = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);

    if (!nativeWindow)
    {
        glfwTerminate();
        throw std::exception("Cannot create GLFW window");
    }

    glfwMakeContextCurrent(nativeWindow);
    glfwSwapInterval(1);
}

bool Window::OnUpdate(double dt)
{
    glfwPollEvents();

    // return renderContext->OnTick(dt) && !glfwWindowShouldClose(nativeWindow);
    return !glfwWindowShouldClose(nativeWindow);
}

void Window::DrawContent()
{
    Frontend::Renderer& r = GetRenderer();
    glfwSwapBuffers(nativeWindow);
    glfwGetFramebufferSize(nativeWindow, &width, &height);

    r.SetViewport(0, 0, width, height); // Just in case another renderer reset the viewport
    r.Clear();
}

void Window::OnShutdown()
{
    glfwDestroyWindow(nativeWindow);
    glfwTerminate();
}
} // namespace GLFW
} // namespace Bored
