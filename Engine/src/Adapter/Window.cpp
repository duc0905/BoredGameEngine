#include "Window.h"
#include <exception>

namespace Bored {
    namespace Window {
        static void error_callback(int error, const char* description)
        {
            fprintf(stderr, "Error: %s\n", description);
        }

        Window::Window()
        {
            if (!glfwInit())
                throw std::exception("Cannot initialize GLFW");

            glfwSetErrorCallback(error_callback);
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
        }

        Window::~Window()
        {
            glfwDestroyWindow(nativeWindow);
            glfwTerminate();
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

        unsigned int Window::GetWidth() const {
            return width;
        }

        unsigned int Window::GetHeight() const {
            return height;
        }

        bool Window::OnTick(double dt) {
            glfwPollEvents();
            glfwSwapBuffers(nativeWindow);

            return renderContext->OnTick(dt) && !glfwWindowShouldClose(nativeWindow);
        }

        void Window::SwapBuffer() {
            glfwSwapBuffers(nativeWindow);
        }

        void Window::PollEvents() {
            glfwPollEvents();
        }

        bool Window::ShouldClose() {
            return glfwWindowShouldClose(nativeWindow);
        }
    }
}
