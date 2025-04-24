#pragma once
#include "Input.hpp"
#include <glad/glad.h>
// Ehhhhh works on Arch with capitalized
#include <GLFW/glfw3.h>
// Works on windows with lowercase
// #include <glfw/glfw3.h>

#include "../Renderer/Shader/Shader.hpp"
#include "../Renderer/Texture/OGL_Texture.hpp"
#include "Listeners.hpp"
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

  ~Window();

  /**
   * Check the state of the window.
   *
   * @return true if the window should be closed.
   */
  bool ShouldStop() const;

  /**
   * Render the texture to the whole window.
   *
   * @note Should only be called once per frame.
   *
   * @param texture The texture the be rendered to the whole screen.
   *
   * @sa I_Renderer3D
   */
  void Render(I_Texture2D *texture);

  /**
   * Wait for any event to occur to save resources.
   *
   * @param timeout Maximum amount of time to wait in seconds.
   */
  void WaitEvents(float timeout) const;

  /**
   * Poll for window events.
   */
  void PollEvents() const;

  /**
   * Register a listener for the FrameBufferSize event.
   *
   * @param listener The listener.
   */
  void AddFrameBufferSizeListener(FrameBufferSizeListener* listener);

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

  std::vector<FrameBufferSizeListener*> m_fbsListeners;

private:
  std::unique_ptr<OGL_Texture2D> m_renderTexture;
  std::unique_ptr<Shader> m_screenShader;
  GLuint m_screenVao;

private:
  bool m_debug = true;
}; // Renderer
