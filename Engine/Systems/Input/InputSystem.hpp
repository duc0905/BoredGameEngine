#pragma once

#include "../../Components/InputComponent.hpp"
#include "../I_System.hpp"
#include "IOService.hpp"

namespace Bored {
/**
 * A class for handling input from different sources using a simple design.
 *
 * An input event is mapped to a "action". An action can have a handler. The
 * user can define the mapping between the input event and the action and the
 * mapping between the action and the handler.
 *
 * Different inputs can be mapped to the same action, which will be handled by
 * the same handler.
 *
 * @todo handles text input, scroll input, custom USB/Wifi/Bluetooth input.
 *
 */
class InputSystem : public I_System {
 public:
  /**
   * @param io_service IOService - the wrapper for GLFW.
   */
  InputSystem(IOService& io_service);

  ~InputSystem() = default;

  /**
   * Handles the event.
   *
   * Allowing user to use/define the input handling system suitable for their
   * purpose per scene. Default: no handling.
   */
  virtual void HandleInputEvents(std::vector<InputEvent>& events) {}

  /**
   * Dispatch the input events to the scene tree from root.
   */
  virtual void OnUpdate(double dt, Scene& scene) override;

  virtual bool ShouldStop(Scene&) override;

  /**
   * Handles mouse movement inputs.
   *
   * Handles mouse movement inputs by dispatching a special mouse handler.
   *
   * @param x the x position of the mouse relative to the left edge of the
   * content area.
   *
   * @param y the y position of the mouse relative to the top edge of the
   * content area.
   *
   */
  inline void HandleCursorPosition(int x, int y);

  /**
   * Handles mouse button inputs.
   *
   * Handles mouse button inputs by mapping key to actions and call the
   * corresponding action handler for each action.
   *
   * @param button The GLFW mouse button code.
   *
   * @param action The GLFW action enum, one of GLFW_PRESS, GLFW_RELEASE.
   *
   * @param mods The GLFW key modifier bits.
   *
   * @sa HandleKey
   *
   */
  inline void HandleMouseButton(int button, int action, int mods);

  /**
   * Handles key inputs.
   *
   * Handles key input from keyboard by mapping key to actions and call the
   * corresponding action handler for each action.
   *
   * @param key The GLFW key code.
   *
   * @param action The GLFW action enum, one of GLFW_PRESS, GLFW_REPEAT,
   * GLFW_RELEASE.
   *
   * @param mods The GLFW key modifier bits.
   *
   */
  inline void HandleKey(int key, int action, int mods);

 private:
  IOService& input_service;

  std::vector<InputEvent> eventQueue;

 private:
  int prev_cursor_pos_x, prev_cursor_pos_y;
};

}  // namespace Bored
