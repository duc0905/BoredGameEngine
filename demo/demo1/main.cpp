#include <memory>

#include "MyScene.hpp"

int main() {
  // Scene setup
  std::shared_ptr<Bored::Scene> scene = std::make_shared<MyScene>();

  scene->BuildScene();

  scene->AfterBuildScene();

  scene->GameLoop();

  return 0;
}
