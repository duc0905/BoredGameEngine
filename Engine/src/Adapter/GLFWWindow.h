#pragma once
#include "Window.h"
#include "Render.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace Bored
{
namespace Render
{
class Manager; // Forward declare
}
} // namespace Bored

namespace Bored
{
namespace GLFW
{
class Window : public Bored::Window
{
  public:
    Window();
    ~Window();

    void SetWidth(unsigned int);
    void SetHeight(unsigned int);
    void SetTitle(const std::string&);
    void SetFullscreen(bool);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    // Inherited via Module
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;

    GLFWwindow* GetNativeWindow() const
    {
        return nativeWindow;
    }

    void ClearColor()
    {
        glClearColor(0.1f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void SwapBuffer()
    {
        glfwSwapBuffers(nativeWindow);
    }
  private:
    std::string name;
    unsigned int width = 800, height = 600;
    bool fullscreen = false;
    //std::unique_ptr<Bored::Render::Context> renderContext;
    std::unique_ptr<Frontend::Renderer> renderer;

    GLFWwindow* nativeWindow = nullptr;
    std::unique_ptr<Render::Context> context;
};
} // namespace Window
} // namespace Bored
