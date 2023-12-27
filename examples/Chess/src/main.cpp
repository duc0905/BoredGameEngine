#include "Bored.hpp"

int main() {
  Bored::GLFW::Window window;

  window.SetHeight(1080);
  window.SetWidth(1920);
  window.SetTitle("Chess");
  window.UseRenderContext(Bored::Render::OGL::Context::GetDefault());
  window.GetRenderer().SetClearColor({0.7f, 0.3f, 0.4f});
  window.OnSetup();

  bool isRunning = true;
  while (isRunning) {
    isRunning &= window.OnUpdate(0.16f);

    window.DrawContent();
  }

  window.OnShutdown();
  return 0;
}
