#include "imgui.h"
#include <stdio.h>

namespace Bored {
  namespace Editor {
    class BoredWindow {
    public:
      BoredWindow() = default;
      ~BoredWindow() = default;
      BoredWindow(const char *title, int width, int height);

      virtual void Init() = 0;
      virtual void Update() = 0;
      virtual void Render() = 0;
      virtual void Shutdown() = 0;

      virtual bool IsRunning() const = 0;
    };

  } // namespace Editor
} // namespace Bored  