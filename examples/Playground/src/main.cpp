#include <iostream>
#include <memory>
#include "Adapter/OGL.h"
#include "PlaygroundConfig.h"

#define PrintMacro(mac) std::cout << "Value of " << #mac << ": " << mac << std::endl;

int main() {
  try {
    std::cout << "Hello from playground!" << std::endl;
    PrintMacro(BUILD_TYPE);
    PrintMacro(SOURCE_DIR);
    PrintMacro(PROJECT_VERSION_MAJOR);
    PrintMacro(PROJECT_VERSION_MINOR);
    PrintMacro(GAME_TITLE);

    Bored::Window::Window window;
    window.SetWidth(600);
    window.SetHeight(400);
    //window.SetFullscreen(true);
    window.SetTitle(GAME_TITLE);
    window.Start<Bored::Render::OGL::Context>();

    bool isRunning = true;

    while (isRunning) {
      glViewport(0, 0, window.GetWidth(), window.GetHeight());
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(0.2f, 0.5f, 0.7f, 1.0f);

      isRunning = isRunning && window.OnTick(0.1f);
    }
  }
  catch (std::exception e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
