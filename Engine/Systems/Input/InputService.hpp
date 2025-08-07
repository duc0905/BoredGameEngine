#pragma once

#include <GLFW/glfw3.h>
#include <functional>

namespace Bored {
enum class CursorMode { VISIBLE, CAPTURED, DISABLED, HIDDEN };

/**
 * Wrapper for GLFW input functionalities
 */
class InputService {

public:
  InputService(GLFWwindow *window) : window(window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, &InputService::KeyCallback);
    glfwSetCursorPosCallback(window, &InputService::CursorPosCallback);
    glfwSetMouseButtonCallback(window, &InputService::MouseButtonCallback);
    glfwSetFramebufferSizeCallback(window,
                                   &InputService::FrameBufferSizeCallback);
  }

  void SetCursorMode(CursorMode mode) {
    switch (mode) {
    case CursorMode::VISIBLE:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;
    case CursorMode::CAPTURED:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
      break;
    case CursorMode::HIDDEN:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
      break;
    case CursorMode::DISABLED:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
      break;
    }
  }

  void PollEvents() { glfwPollEvents(); }

  bool ShouldStop() { return glfwWindowShouldClose(window); }

  std::pair<int, int> GetCursorPos() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return {(int)x, (int)y};
  }

  bool IsKeyPressed(int key) const { return glfwGetKey(window, key); }

  void SetKeyHandler(std::function<void(int, int, int)> handler) {
    key_callback = handler;
  }

  void SetCursorPosHandler(std::function<void(int, int)> handler) {
    cursor_pos_callback = handler;
  }

  void SetFrameBufferSizeHandler(std::function<void(int, int)> handler) {
    frame_buffer_size_callback = handler;
  }

  void SetMouseButtonHandler(std::function<void(int, int, int)> handler) {
    mouse_button_callback = handler;
  }

  static void KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods) {
    InputService *this_ =
        static_cast<InputService *>(glfwGetWindowUserPointer(window));

    if (this_->key_callback)
      this_->key_callback(key, action, mods);
  }

  static void CursorPosCallback(GLFWwindow *window, double x, double y) {
    InputService *this_ =
        static_cast<InputService *>(glfwGetWindowUserPointer(window));

    if (this_->cursor_pos_callback)
      this_->cursor_pos_callback((int)x, (int)y);
  }

  static void FrameBufferSizeCallback(GLFWwindow *window, int width,
                                      int height) {
    InputService *this_ =
        static_cast<InputService *>(glfwGetWindowUserPointer(window));

    if (this_->frame_buffer_size_callback)
      this_->frame_buffer_size_callback(width, height);
  }

  static void MouseButtonCallback(GLFWwindow *window, int key, int action,
                                  int mods) {
    InputService *this_ =
        static_cast<InputService *>(glfwGetWindowUserPointer(window));

    if (this_->mouse_button_callback)
      this_->mouse_button_callback(key, action, mods);
  }

private:
  GLFWwindow *window;

public:
  std::function<void(int, int, int)> key_callback;
  std::function<void(int, int)> cursor_pos_callback;
  std::function<void(int, int)> frame_buffer_size_callback;
  std::function<void(int, int, int)> mouse_button_callback;
};
} // namespace Bored
