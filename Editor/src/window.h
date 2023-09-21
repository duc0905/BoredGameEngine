#pragma once
#include <imgui.h>

namespace Bored {
namespace Editor {
class BoredWindow {
 public:
  BoredWindow() = default;
  ~BoredWindow() = default;
  BoredWindow(const char* title, int width, int height, bool open)
      : _title(title), _width(width), _height(height), _open(open){};
  BoredWindow(const char* title, int width, int height)
      : BoredWindow(title, width, height, true){};

  void Create() {
    ImGui::SetNextWindowSize(ImVec2(_width, _height), ImGuiCond_Once);
    ImGui::Begin(_title, &_open, 0);
    Update();
    ImGui::End();
  }

  void SetOpen(bool open) { _open = open; };

  virtual void Init() = 0;
  virtual void Update() = 0;
  virtual void Render() = 0;
  virtual void Shutdown() = 0;

  virtual bool IsRunning() const = 0;

 private:
  const char* _title;
  bool _open;
  int _width, _height;
};

}  // namespace Editor
}  // namespace Bored