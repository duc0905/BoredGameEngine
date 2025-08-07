#pragma once

#include <memory>
#include "../Scene/Node.hpp"

namespace Bored {
enum class InputType {
  KEY_DOWN,
  KEY_REPEAT,
  KEY_UP,
  MOUSE_MOVE,
  MOUSE_BUTTON_DOWN,
  MOUSE_BUTTON_UP,
  MOUSE_SCROLL
};

struct InputEvent {
  InputType type;

  bool handled = false;

  union {
    struct {
      int keyCode;
      int mods;
    } key;

    struct {
      int x, y;
      int dx, dy;
    } mouseMove;

    struct {
      int button;
      int mods;
    } mouseButton;

    struct {
      float value;
    } scroll;
  };
};

class InputHandler {
public:
  virtual void OnInput(double dt, InputEvent &event, std::shared_ptr<Node> node) = 0;
};

struct InputComponent {
  std::shared_ptr<InputHandler> input_handler;
};
} // namespace Bored
