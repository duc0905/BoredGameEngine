#pragma once

#include "../../Components/InputComponent.hpp"
#include "../../Scene/Scene.hpp"
#include "../I_System.hpp"
#include "InputService.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <memory>

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
 * @todo handles text input, scroll input
 *
 */
class Input : public I_System {
public:
  Input(InputService &input_service) : input_service(input_service) {
    auto pos = input_service.GetCursorPos();
    prev_cursor_pos_x = pos.first;
    prev_cursor_pos_y = pos.second;

    input_service.SetKeyHandler(
        std::bind(&Input::HandleKey, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));

    input_service.SetCursorPosHandler(std::bind(&Input::HandleCursorPosition,
                                                this, std::placeholders::_1,
                                                std::placeholders::_2));

    input_service.SetMouseButtonHandler(
        std::bind(&Input::HandleMouseButton, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));
  }

  ~Input() = default;

  /**
   * Dispatch the input events to the scene tree from root.
   */
  virtual void OnUpdate(double dt, Scene &scene) override {
    input_service.PollEvents();

    // Get all input components, and then do stuff
    for (auto &event : eventQueue) {
      scene.Traverse([&event, dt](std::shared_ptr<Node> node) {
        if (event.handled)
          return;

        if (node->HasComponent<InputComponent>()) {
          auto &input_comp = node->GetComponent<InputComponent>();

          if (input_comp.input_handler)
            input_comp.input_handler->OnInput(dt, event, node);
        }
      });
    }

    eventQueue.clear();
  }

  virtual bool ShouldStop(Scene &) override {
    return input_service.ShouldStop();
  }

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
  inline void HandleCursorPosition(int x, int y) {
    InputEvent e;
    e.type = InputType::MOUSE_MOVE;
    e.mouseMove = {x, y, x - prev_cursor_pos_x, y - prev_cursor_pos_y};

    prev_cursor_pos_x = x;
    prev_cursor_pos_y = y;

    eventQueue.push_back(e);
  }

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
  inline void HandleMouseButton(int button, int action, int mods) {
    InputEvent e;
    if (action == GLFW_PRESS) {
      e.type = InputType::MOUSE_BUTTON_DOWN;
    } else {
      e.type = InputType::MOUSE_BUTTON_UP;
    }

    e.mouseButton.button = button;
    e.mouseButton.mods = mods;

    eventQueue.push_back(e);
  }

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
  inline void HandleKey(int key, int action, int mods) {
    InputEvent e;

    if (action == GLFW_PRESS) {
      e.type = InputType::KEY_DOWN;
    } else if (action == GLFW_REPEAT) {
      e.type = InputType::KEY_REPEAT;
    } else {
      e.type = InputType::KEY_UP;
    }

    e.key.keyCode = key;
    e.key.mods = mods;

    eventQueue.push_back(e);
  }

public:
  // std::unordered_map<Key, std::string> m_keyToAction; /**< Maps Key to action
  // */ std::unordered_map<std::string, std::unique_ptr<ActionHandler<>>>
  //     m_actionHandlers; /**< action handler */
  // std::unique_ptr<ActionHandler<double, double>>
  //     m_cursorPosHandler; /**< Handler for mouse position events */

private:
  InputService &input_service;

  std::vector<InputEvent> eventQueue;

private:
  int prev_cursor_pos_x, prev_cursor_pos_y;
};

} // namespace Bored
