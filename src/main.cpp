#include "Window/Window.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  Window window(SCR_WIDTH, SCR_HEIGHT);

  window.Run();

  return 0;
}
