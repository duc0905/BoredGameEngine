#include "Scene2D.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc <= 2) {
    std::cerr << "Require filepath and glyph index" << std::endl;
    return 1;
  }

  Scene2D scene(argv[1], std::stoi(argv[2]));

  scene.BuildScene();

  scene.AfterBuildScene();

  scene.GameLoop();

  return 0;
}
