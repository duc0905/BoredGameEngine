#include <exception>
#include "Window.h"
#include "OGL.h"
#include "GLFWWindow.h"

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

void Window::SetWidth(unsigned int w)
{
    width = w;
}

void Window::SetHeight(unsigned int h)
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

unsigned int Window::GetWidth() const
{
    return width;
}

unsigned int Window::GetHeight() const
{
    return height;
}

bool Window::OnUpdate(double dt)
{
    glfwPollEvents();
    glfwSwapBuffers(nativeWindow);
    ClearColor();

    // return renderContext->OnTick(dt) && !glfwWindowShouldClose(nativeWindow);
    return !glfwWindowShouldClose(nativeWindow);
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

    // renderContext = std::make_unique<T>(this);
    renderer = std::make_unique<Frontend::Renderer>(new Render::OGL::Context());
}
void Window::OnShutdown()
{
    glfwDestroyWindow(nativeWindow);
    glfwTerminate();
}
} // namespace Window
} // namespace Bored
