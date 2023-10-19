#include <exception>
#include "Window.h"
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>
#include "../GameStruct.hpp"

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
    input = std::make_unique<Input>(this);
    game.window = this;
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

void Window::DrawContent()
{
    Frontend::Renderer& r = GetRenderer();
    glfwSwapBuffers(nativeWindow);
    glfwGetFramebufferSize(nativeWindow, &width, &height);

    r.SetViewport(0, 0, width, height); // Just in case another renderer reset the viewport
    r.Clear();
}

void Window::PollEvents()
{
    glfwPollEvents();
}

Frontend::Input* Window::GetInput()
{
    return input.get();
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

    // Setup input callbacks
    input->OnSetup();
}

bool Window::OnUpdate(double dt)
{
    input->OnUpdate(dt);

    return !glfwWindowShouldClose(nativeWindow);
}

void Window::OnShutdown()
{
    glfwDestroyWindow(nativeWindow);
    glfwTerminate();
}

Input::Input(Window* w): Frontend::Input(w), window(w)
{
}

Input::~Input()
{
}

void Input::OnSetup()
{
    Frontend::Input::OnSetup();

    glfwSetKeyCallback(window->GetNativeWindow(),
    [](GLFWwindow* w, int key, int scan, int action, int mods) {
        // TODO
        // GetInputInstance()
        game.window->GetInput()->EvaluateKey(Input::GetKey(key), Input::GetAction(action), Input::GetMods(mods), 1.0f);
        if (action == GLFW_PRESS) {
            printf("Pressed key: %d\n", key);
        }
    });
}

Frontend::Key Input::GetKey(int k)
{
    // TODO
    return Frontend::Key::KEY_UNKNOWN;
}

Frontend::Action Input::GetAction(int a)
{
    // TODO
    return Frontend::Action::UNKNOWN;
}

int Input::GetMods(int m)
{
    // TODO
    return 0;
}

void Input::SetCursorImage(unsigned char* image, unsigned int width, unsigned int height)
{
    throw std::exception("Not implemented");
}
void Input::EnableCursor()
{
    throw std::exception("Not implemented");
}
void Input::DisableCursor()
{
    throw std::exception("Not implemented");
}

} // namespace GLFW
} // namespace Bored
