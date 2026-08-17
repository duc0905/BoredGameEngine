#pragma once

#include <map>

#include "../../Components/InputComponent.hpp"
#include "../I_System.hpp"
#include "IOService.hpp"

namespace Bored {
/**
 * A class for handling physical input from different sources using context
 * mapping input handling.
 *
 * An input event is mapped to an "action". An action can have a handler. The
 * user can define the mapping between the input event and the action and the
 * mapping between the action and the handler.
 *
 * A context contains a mapping between physical input and an action and a
 * mapping between an action and its funciton handler.
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

  // Mapping physical event to action name
  typedef std::map<InputType, std::string> InputActionMap;

  typedef std::function<void(InputEvent&)> InputHandler;

  // Mapping action to function handler
  // @todo Debate whether InputEvent should be passed to function handler
  // everytime.
  typedef std::map<std::string, InputHandler> ActionHandlerMap;

  class InputCtx {
    friend class InputSystem;

   public:
    void RegisterInput(InputType type, const std::string& action) {
      itoa_map.insert({type, action});
    }

    void RegisterHandler(const std::string& action, InputHandler handler) {
      atoh_map.insert({action, handler});
    }

   private:
    InputActionMap itoa_map;
    ActionHandlerMap atoh_map;
  };

  /**
   * Handles the event.
   *
   * Use Context mapping for handling input event.
   */
  void HandleInputEvents(std::vector<InputEvent>& events);

  /**
   * Create a new input context.
   *
   * @return the index of the newly created context and the reference to the
   * context.
   */
  std::pair<int, InputCtx&> CreateContext();

  /**
   * Change the active input context.
   *
   * @param ctx_idx uint the index of the new context.
   */
  void SwitchContext(unsigned int ctx_idx);

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

 private:  // Context mapping input handling design
  std::vector<InputCtx> contexts;
  unsigned int active_ctx_idx = 0;
};

}  // namespace Bored
