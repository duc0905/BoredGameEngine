#include "Scene2D.hpp"

int main() {
  Scene2D scene;

  scene.BuildScene();

  scene.AfterBuildScene();

  scene.GameLoop();

  return 0;
}
