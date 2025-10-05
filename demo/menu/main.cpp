#include "Scene2D.hpp"
#include "Utils/AssetManager.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  // Scene2D scene;
  //
  // scene.BuildScene();
  //
  // scene.AfterBuildScene();
  //
  // scene.GameLoop();

  if (argc <= 1) {
    std::cout << "Require filepath" << std::endl;
    return 1;
  }

  Bored::AssetManager& am = Bored::AssetManager::GetInstance();

  am.LoadFontTTF(argv[1]);

  return 0;
}
