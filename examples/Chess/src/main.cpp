#include "Bored.hpp"
#include <chrono>

/* TODO: Try to use ChessLogic.dll in this file */

int main() {
  auto* window = Bored::GLFW::Window::GetInstance();

  window->SetTitle("Bored Chess");
  window->SetWidth(800);
  window->SetHeight(800);

  window->OnSetup();
  window->UseRenderContext(Bored::Render::OGL::Context::GetDefault());

  Bored::GameLogic gl;

  Bored::Util::DLLLoader mod_loader;
  mod_loader.Load("ChessLogic.dll");
  auto x = mod_loader.GetIntance<Bored::Module>();
  gl.AddModule(x);

  bool isRunning = true;
  double dt = 0;
  std::chrono::steady_clock::time_point start, end;

  start = std::chrono::steady_clock::now();

  while (isRunning) {
    end = std::chrono::steady_clock::now();
    dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    isRunning &= window->OnUpdate(dt);
    window->DrawContent();

    // isRunning &= logic.OnUpdate(dt);
    start = end;
  }

  return 0;
}
