#include <exception>
#include <iostream>
#include <memory>
#include "Window.h"
#include "GLFWWindow.h"
#include <GLFW/glfw3.h>
// #include "../GameStruct.hpp"

namespace Bored
{
namespace GLFW
{
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

std::unique_ptr<Window> Window::instancee_ = nullptr;

Window::Window()
{
    input = std::make_unique<Input>(this);
    // id = game.AddWindow(this);
}

Window* Window::GetInstance() {
    if (!instancee_)
        instancee_ = std::unique_ptr<Window>(new Window());
    return instancee_.get();
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

void Window::NewFrame()
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

    NewFrame();

    return !glfwWindowShouldClose(nativeWindow);
}

void Window::OnShutdown()
{
    glfwDestroyWindow(nativeWindow);
    glfwTerminate();
}

Input::Input(Window* w) : Frontend::Input(w), window(w)
{
}

Input::~Input()
{
}

void Input::OnSetup()
{
    Frontend::Input::OnSetup();

    glfwSetKeyCallback((GLFWwindow*)window->GetNativeWindow(),
                       [](GLFWwindow* w, int key, int scan, int action, int mods) {
                       Window::GetInstance()->GetInput()->EvaluateKey(GetKey(key), GetAction(action), GetMods(mods), 1.0f);

                       std::cout << "Receiving input " << GetKey(key) << " " << scan << " " << action << " " << mods << std::endl;

                       });
}

Frontend::Key Input::GetKey(int k)
{
    switch (k)
    {
        case GLFW_KEY_1:
            return Frontend::KEY_1;
        case GLFW_KEY_2:
            return Frontend::KEY_2;
        case GLFW_KEY_3:
            return Frontend::KEY_3;
        case GLFW_KEY_4:
            return Frontend::KEY_4;
        case GLFW_KEY_5:
            return Frontend::KEY_5;
        case GLFW_KEY_6:
            return Frontend::KEY_6;
        case GLFW_KEY_7:
            return Frontend::KEY_7;
        case GLFW_KEY_8:
            return Frontend::KEY_8;
        case GLFW_KEY_9:
            return Frontend::KEY_9;
        case GLFW_KEY_0:
            return Frontend::KEY_0;
        case GLFW_KEY_Q:
            return Frontend::KEY_Q;
        case GLFW_KEY_W:
            return Frontend::KEY_W;
        case GLFW_KEY_E:
            return Frontend::KEY_E;
        case GLFW_KEY_R:
            return Frontend::KEY_R;
        case GLFW_KEY_T:
            return Frontend::KEY_T;
        case GLFW_KEY_Y:
            return Frontend::KEY_Y;
        case GLFW_KEY_U:
            return Frontend::KEY_U;
        case GLFW_KEY_I:
            return Frontend::KEY_I;
        case GLFW_KEY_O:
            return Frontend::KEY_O;
        case GLFW_KEY_P:
            return Frontend::KEY_P;
        case GLFW_KEY_A:
            return Frontend::KEY_A;
        case GLFW_KEY_S:
            return Frontend::KEY_S;
        case GLFW_KEY_D:
            return Frontend::KEY_D;
        case GLFW_KEY_F:
            return Frontend::KEY_F;
        case GLFW_KEY_G:
            return Frontend::KEY_G;
        case GLFW_KEY_H:
            return Frontend::KEY_H;
        case GLFW_KEY_J:
            return Frontend::KEY_J;
        case GLFW_KEY_K:
            return Frontend::KEY_K;
        case GLFW_KEY_L:
            return Frontend::KEY_L;
        case GLFW_KEY_Z:
            return Frontend::KEY_Z;
        case GLFW_KEY_X:
            return Frontend::KEY_X;
        case GLFW_KEY_C:
            return Frontend::KEY_C;
        case GLFW_KEY_V:
            return Frontend::KEY_V;
        case GLFW_KEY_B:
            return Frontend::KEY_B;
        case GLFW_KEY_N:
            return Frontend::KEY_N;
        case GLFW_KEY_M:
            return Frontend::KEY_M;
        case GLFW_KEY_SPACE:
            return Frontend::KEY_SPACE;
        case GLFW_KEY_LEFT_SHIFT:
            return Frontend::KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:
            return Frontend::KEY_LEFT_CONTROL;

        // mouse
        case GLFW_MOUSE_BUTTON_1:
            return Frontend::KEY_MB_1;
        case GLFW_MOUSE_BUTTON_2:
            return Frontend::KEY_MB_2;
        case GLFW_MOUSE_BUTTON_3:
            return Frontend::KEY_MB_3;
        case GLFW_MOUSE_BUTTON_4:
            return Frontend::KEY_MB_4;
        case GLFW_MOUSE_BUTTON_5:
            return Frontend::KEY_MB_5;
        case GLFW_MOUSE_BUTTON_6:
            return Frontend::KEY_MB_6;
        case GLFW_MOUSE_BUTTON_7:
            return Frontend::KEY_MB_7;
        case GLFW_MOUSE_BUTTON_8:
            return Frontend::KEY_MB_8;

        // default
        default:
            return Frontend::KEY_UNKNOWN;
    }
}

Frontend::Action Input::GetAction(int a)
{
    switch (a)
    {
        case GLFW_PRESS:
            return Frontend::PRESS;
        case GLFW_REPEAT:
            return Frontend::REPEAT;
        case GLFW_RELEASE:
            return Frontend::RELEASE;
        default:
            return Frontend::UNKNOWN;
    }
}

int Input::GetMods(int m)
{
    int ans = 0;
    if (m & GLFW_MOD_CONTROL)
        ans |= Frontend::CTRL;
    if (m & GLFW_MOD_SHIFT)
        ans |= Frontend::SHIFT;
    if (m & GLFW_MOD_ALT)
        ans |= Frontend::ALT;
    return ans;
}

void Input::SetCursorImage(unsigned char* image, unsigned int width, unsigned int height)
{
    throw std::exception("Not implemented");
}
void Input::EnableCursor()
{
    if (window)
    {
        glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
void Input::DisableCursor()
{
    if (window)
    {
        glfwSetInputMode((GLFWwindow*)window->GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

} // namespace GLFW
} // namespace Bored
