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

    void SetWidth(unsigned int) override;
    void SetHeight(unsigned int) override;
    void SetTitle(const std::string&) override;
    void SetFullscreen(bool) override;

    unsigned int GetWidth() const override;
    unsigned int GetHeight() const override;

    // Inherited via Module
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;

    GLFWwindow* GetNativeWindow() const
    {
        return nativeWindow;
    }

  private:
    std::string name;
    int width = 800, height = 600;
    bool fullscreen = false;
    // std::unique_ptr<Bored::Render::Context> renderContext;
    std::unique_ptr<Frontend::Renderer> renderer;

    GLFWwindow* nativeWindow = nullptr;
    std::unique_ptr<Render::Context> context;
};
} // namespace GLFW
} // namespace Bored
