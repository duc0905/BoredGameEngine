#pragma once
#include "Input.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <functional>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Window {
public:
  /**
   * Create a Window.
   *
   * Uses GLFW to create a Window.
   *
   * @param w The width of the window.
   *
   * @param h The height of the window.
   *
   */
  Window(const int &w, const int &h);

  ~Window() {
    // Cleanup code here
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  /**
   * Run the game-loop
   */
  void Run();

private:
  /**
   * This function is called in the game-loop.
   *
   * Renders the scene.
   */
  void Render();

private: // Callback handler functions
  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);

  void HandleKey(int key, int scancode, int action, int mods);

  static void GLAPIENTRY DebugOutputCallback(GLenum source, GLenum type,
                                             unsigned int id, GLenum severity,
                                             GLsizei length,
                                             const char *message,
                                             const void *userParam);

  static void CursorPosCallback(GLFWwindow *window, double x, double y);

  void HandleCursorPos(double x, double y);

  void HandleDebugMessage(GLenum source, GLenum type, unsigned int id,
                          GLenum severity, GLsizei length, const char *message);

  static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height);

  void HandleFrameBufferSize(int width, int height);

  static void HandleGLFWError(int error, const char *description);

private:
  int m_width, m_height;
  GLFWwindow *m_window;

private: // Input section
  Input m_input;
  GLFWkeyfun m_prevKeyCallback = nullptr;

private:
  bool m_debug = true;
}; // Renderer
