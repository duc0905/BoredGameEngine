#include <chrono>
#include <glm/ext/scalar_constants.hpp>
#include <glm/geometric.hpp>
#include <memory>

#include "MyScene.hpp"

int main() {
  // Scene setup
  std::shared_ptr<Bored::Scene> scene = std::make_shared<MyScene>();

  scene->BuildScene();

  scene->AfterBuildScene();

  bool running = true;
  std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();

  // Main loop
  while (running) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();

    // Elapsed time since last frame in seconds
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
                   .count() /
               1000.0f;

    // Update scene here
    scene->Update(dt);

    running &= !scene->ShouldStop();

    prev = now;

    // window->WaitEvents(0.0016f);
  }

  return 0;
}
