#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// clang-format on
#include <functional>
#include <iostream>
#include <stdexcept>

#include "../Renderer/Shader/Shader.hpp"
#include "../Renderer/Texture/OGL_Texture.hpp"

namespace Bored {
enum class CursorMode { VISIBLE, CAPTURED, DISABLED, HIDDEN };

/**
 * Wrapper for GLFW input functionalities
 */
class IOService {
public:
  bool m_debug = true;

public:
  IOService(int width = 800, int height = 600);

  void SetCursorMode(CursorMode mode);

  void PollEvents();

  /**
   * Render the texture to the whole window.
   *
   * @note Should only be called once per frame.
   *
   * @param texture The texture the be rendered to the whole screen.
   *
   * @sa I_Renderer3D
   */
  void Render(std::shared_ptr<I_Texture2D> texture);

public: // State query
  bool ShouldStop() const;

  std::pair<int, int> GetCursorPos() const;

  bool IsKeyPressed(int key) const;

  std::pair<int, int> GetFrameBufferSize() const;

public: // GLFW input callbacks
  void SetKeyHandler(std::function<void(int, int, int)> handler);

  void SetCursorPosHandler(std::function<void(int, int)> handler);

  void SetFrameBufferSizeHandler(std::function<void(int, int)> handler);

  void SetMouseButtonHandler(std::function<void(int, int, int)> handler);

  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);

  static void CursorPosCallback(GLFWwindow *window, double x, double y);

  static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height);

  static void MouseButtonCallback(GLFWwindow *window, int key, int action,
                                  int mods);

private: // OpenGL error handling code
  void HandleDebugMessage(GLenum source, GLenum type, unsigned int id,
                          GLenum severity, GLsizei length, const char *message);

  static void GLAPIENTRY DebugOutputCallback(GLenum source, GLenum type,
                                             unsigned int id, GLenum severity,
                                             GLsizei length,
                                             const char *message,
                                             const void *userParam);
  static void HandleGLFWError(int error, const char *description);

private:
  GLFWwindow *window;

private:
  std::unique_ptr<OGL_Texture2D> m_renderTexture;
  std::unique_ptr<Shader> m_screenShader;
  GLuint m_screenVao;

public:
  std::function<void(int, int, int)> key_callback;
  std::function<void(int, int)> cursor_pos_callback;
  std::function<void(int, int)> frame_buffer_size_callback;
  std::function<void(int, int, int)> mouse_button_callback;
};
} // namespace Bored
