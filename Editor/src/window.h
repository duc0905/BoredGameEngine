#pragma once
#include <imgui.h>

namespace Bored {
  namespace Editor {
    class BoredWindow {
    public:
      BoredWindow() = default;
      ~BoredWindow() = default;
      BoredWindow(const char *title, int width, int height);

      void Create() {
          ImGui::Begin(title, &open, 0);

          // tim cach set width voi height

          Init();

          ImGui::End();
      }

      virtual void Init() = 0;
      virtual void Update() = 0;
      virtual void Render() = 0;
      virtual void Shutdown() = 0;

      virtual bool IsRunning() const = 0;
    private:
      const char* title;
      bool open;
      int width, height;
    };

  } // namespace Editor
} // namespace Bored  