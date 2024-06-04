#pragma once
#include "Window.h"
#include "Render.h"
#include "../Frontend/Input.hpp"
#include <memory>
#include <string>

struct GLFWwindow;
namespace Bored
{
namespace Render
{
// Forward declare
class Manager;
} // namespace Render
} // namespace Bored

namespace Bored
{
namespace GLFW
{
class Input;
class Window : public Bored::Window
{
public:
  // Window();
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
  void PollEvents() override;

  void* GetNativeWindow() override
  {
    return nativeWindow;
  }

  Frontend::Input* GetInput() override;

  static Window* GetInstance();
private:
  Window();
  static std::unique_ptr<Window> instance_;

private:
  std::string name;
  int width = 800, height = 600;
  bool fullscreen = false;

  GLFWwindow* nativeWindow = nullptr;

  std::unique_ptr<Input> input;
};

class Input : public Frontend::Input
{
public:
  Input(Window* w);
  ~Input();
  virtual void SetCursorImage(unsigned char* image, unsigned int width, unsigned int height) override;
  virtual void EnableCursor() override;
  virtual void DisableCursor() override;

  virtual void OnSetup() override;

private:
  static Frontend::Key GetKey(int k);
  static Frontend::Action GetAction(int a);
  static int GetMods(int m);

  Window* window;
};
} // namespace GLFW
} // namespace Bored
