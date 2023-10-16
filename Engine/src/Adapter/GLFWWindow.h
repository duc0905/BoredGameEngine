#pragma once
#include "Window.h"
#include "Render.h"
#include <memory>
#include <string>

struct GLFWwindow;
namespace Bored
{
namespace Render
{
// Forward declare
class Manager;
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

    void SetWidth(int) override;
    void SetHeight(int) override;
    void SetTitle(const std::string&) override;
    void SetFullscreen(bool) override;

    int GetWidth() const override;
    int GetHeight() const override;

    // Inherited via Module
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;

    void DrawContent() override;

    GLFWwindow* GetNativeWindow() const
    {
        return nativeWindow;
    }

  private:
    std::string name;
    int width = 800, height = 600;
    bool fullscreen = false;

    GLFWwindow* nativeWindow = nullptr;
};
} // namespace GLFW
} // namespace Bored
