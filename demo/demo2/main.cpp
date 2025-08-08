#include <memory>

#include "MyScene.hpp"

int main() {
  // Scene setup
  std::shared_ptr<Bored::Scene> scene = std::make_shared<MyScene>();

  try {
    scene->BuildScene();

    scene->AfterBuildScene();

    scene->GameLoop();
  } catch (const std::exception &e) {
    std::cerr << "[Error]: " << e.what() << std::endl;
  }

  return 0;
}
