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
      isRunning = isRunning && window.OnTick(0.1f);
    }
  }
  catch (std::exception e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    exit(1);
  }

  return 0;
}
