#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

struct Key {
  int m_keyCode;
  int m_action;
  int m_mods;

  bool operator==(const Key &other) const {
    return (m_keyCode == other.m_keyCode) and (m_action == other.m_action) and
           (m_mods == other.m_mods);
  }
};

template <> struct std::hash<Key> {
  std::size_t operator()(const Key &k) const {
    return (std::size_t)k.m_keyCode ^ ((std::size_t)k.m_action << 10) ^
           ((std::size_t)k.m_mods << 12);
  }
};

enum class InputType {
  KEY_DOWN,
  KEY_UP,
  MOUSE_MOVE,
  MOUSE_BUTTON_DOWN,
  MOUSE_BUTTON_UP,
  MOUSE_SCROLL
};

struct InputEvent {
  InputType type;

  union {
    struct {
      int keyCode;
    } key;

    struct {
      int x, y;
      int dx, dy;
    } mouseMove;

    struct {
      int button;
    } mouseButton;

    struct {
      float value;
    } scroll;
  };
};

class InputDevice {
public:
  virtual ~InputDevice() = default;
  virtual void PollEvents(std::vector<InputEvent> &outEvents) = 0;
};

class InputListener {
public:
  virtual ~InputListener() = default;

  virtual void OnInputEvent(const InputEvent& event) = 0;
};

class InputManager {
public:
  void RegisterDevice(std::unique_ptr<InputDevice> device) {
    devices.push_back(std::move(device));
  }

  void RegisterListener(std::shared_ptr<InputListener> listener) {
    listeners.push_back(listener);
  }

  void PollAndDispatch() {
    eventQueue.clear();
    for (auto& device : devices)
      device->PollEvents(eventQueue);

    for (auto& event : eventQueue)
      for (auto listener : listeners)
        listener->OnInputEvent(event);
  }

private:
  std::vector<std::unique_ptr<InputDevice>> devices;
  std::vector<std::shared_ptr<InputListener>> listeners;
  std::vector<InputEvent> eventQueue;
};

/**
 * Base class for defining an action handler.
 *
 * Defines the HandleAction method for handling actions when the Input invoke an
 * action.
 */
template <class... Args>
class ActionHandler {
public:
  virtual ~ActionHandler() {}
  /**
   * Callback for when particular action is dispatched.
   */
  virtual void HandleAction(Args... args) = 0;
};

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
class Input {
public:
  Input() = default;
  ~Input() = default;

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
  inline void HandleCursorPosition(double x, double y) const {
    if (m_cursorPosHandler) {
      m_cursorPosHandler->HandleAction(x, y);
    }
  }

  /**
   * Handles mouse button inputs.
   *
   * Handles mouse button inputs by mapping key to actions and call the
   * corresponding action handler for each action.
   *
   * @param button The GLFW mouse button code.
   *
   * @param action The GLFW action enum, one of GLFW_PRESS, GLFW_REPEAT,
   * GLFW_RELEASE.
   *
   * @param mods The GLFW key modifier bits.
   *
   * @sa HandleKey
   *
   */
  inline void HandleMouseButton(int button, int action, int mods) const {
    Key k(button, action, mods);

    if (m_keyToAction.find(k) == m_keyToAction.end()) { // No action found
      std::cout << "[Info]: No action found with Key(" << button << ", "
                << action << ", " << mods << ")" << std::endl;
    } else {
      std::string action = m_keyToAction.at(k);
      DispatchAction(action);
    }
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
  inline void HandleKey(int key, int action, int mods) const {
    Key k(key, action, mods);

    if (m_keyToAction.find(k) == m_keyToAction.end()) { // No action found
      std::cout << "[Info]: No action found with Key(" << key << ", " << action
                << ", " << mods << ")" << std::endl;
    } else {
      std::string action = m_keyToAction.at(k);
      DispatchAction(action);
    }
  }

  /**
   * Dispatch/invoke an action
   *
   * Will call the corresponding action handler for the input action if there
   * is.
   *
   * @param action The action to be dispatched
   *
   */
  inline void DispatchAction(const std::string &action) const {
    if (m_actionHandlers.find(action) == m_actionHandlers.end()) {
      std::cout << "[Info]: No action handler found for action '" << action
                << "'" << std::endl;
    } else {
      auto &handler = m_actionHandlers.at(action);
      handler->HandleAction();
    }
  }

  /**
   * Insert an action handler.
   *
   * @param action The name of the action to handle
   *
   * @param handler The handle function for the action
   *
   * @sa DispatchAction
   *
   * @sa HandleKey
   *
   */
  inline void AddActionHandler(const std::string &action,
                               ActionHandler<> *handler) {
    if (m_actionHandlers.find(action) == m_actionHandlers.end()) {
      std::cout << "[Warning]: Overriding action handler for action '" << action
                << "'" << std::endl;
    }

    m_actionHandlers[action].reset(handler);
  }

public:
  std::unordered_map<Key, std::string> m_keyToAction; /**< Maps Key to action */
  std::unordered_map<std::string, std::unique_ptr<ActionHandler<>>>
      m_actionHandlers; /**< action handler */
  std::unique_ptr<ActionHandler<double, double>>
      m_cursorPosHandler; /**< Handler for mouse position events */
};
