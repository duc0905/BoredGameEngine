#include "Renderer/CPU/RayTracer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Renderer/I_Object.hpp"
#include "Renderer/Objects/Sphere.hpp"
#include "Window/Window.hpp"
#include <chrono>
#include <glm/ext/scalar_constants.hpp>
#include <memory>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  Window window(SCR_WIDTH, SCR_HEIGHT);
  CPU_RayTracer renderer(
      Camera(
          new View({0.0f, 0.0f, -3.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}),
          new Perspective(Perspective::GetFOV(1.0f, 2.0f), SCR_WIDTH,
                          SCR_HEIGHT, 1.0f)),
      SCR_WIDTH, SCR_HEIGHT);
  window.AddFrameBufferSizeListener(&renderer);

  // Do the pre running setup

  std::vector<std::shared_ptr<I_Object3D>> objects;

  std::shared_ptr<Sphere> sphere;

  sphere = std::make_shared<Sphere>();
  sphere->Scale({1.0f, 3.0f, 1.0f})
      .RotateZ(glm::pi<float>() / 6.0f)
      .Translate({1.0f, -1.0f, 3.0f});
  objects.push_back(sphere);

  // for (int i = 0; i < 5; i++) {
  //   for (int j = 0; j < 5; j++) {
  //     sphere = std::make_shared<Sphere>();
  //     sphere->Scale({0.1f, 0.1f, 0.1f})
  //         .Translate({-4.0f + i * 1.6f, -4.0f + j * 1.6f, 5.0f});
  //     objects.push_back(sphere);
  //   }
  // }
  // sphere->Scale({0.3f, 0.3f, 0.3f}).Translate({1.0f, 1.0f, 1.0f});
  // objects.push_back(sphere);
  //
  // sphere = std::make_shared<Sphere>();
  // sphere->Scale({0.4f, 0.4f, 0.4f}).Translate({-2.0f, -1.0f, 3.0f});
  // objects.push_back(sphere);

  renderer.SetupObjects(objects);
  std::cout << "sphere matrix: " << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << sphere->m_T[j][i] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << "sphere inv matrix: " << std::endl;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << sphere->m_Tinv[j][i] << " ";
    }
    std::cout << std::endl;
  }

  glm::vec4 p = sphere->m_Tinv * glm::vec4{0.0f, 0.0f, -35.0f, 1.0f};
  std::cout << "p: (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;

  bool running = true;
  std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
  // Main loop
  while (running) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();

    // Elapsed time since last frame in milisecond
    long long dt =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
            .count();

    if (dt >= 2.0f) { // 0.5 FPS
                      // Render here
      auto tex = renderer.Render();
      window.Render(tex.get());

      prev = now;
    }

    // Wait for events for at most 2 seconds
    window.WaitEvents(2.0f);
  }

  return 0;
}
