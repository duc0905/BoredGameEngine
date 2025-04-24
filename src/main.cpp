#include "Renderer/CPU/RayTracer.hpp"
#include "Renderer/Camera/Camera.hpp"
#include "Window/Window.hpp"
#include <chrono>

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

  // Do the pre running setup
  renderer.SetupObjects({});

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

    if (dt >= 160) { // 6 FPS
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
