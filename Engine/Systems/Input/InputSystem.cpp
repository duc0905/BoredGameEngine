#pragma once

#include "InputSystem.hpp"

#include <functional>

#include "../../Scene/Scene.hpp"

namespace Bored {

InputSystem::InputSystem(IOService& io_service) : input_service(io_service) {
  auto pos = io_service.GetCursorPos();
  prev_cursor_pos_x = pos.first;
  prev_cursor_pos_y = pos.second;

  io_service.SetKeyHandler(
      std::bind(&InputSystem::HandleKey, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));

  io_service.SetCursorPosHandler(std::bind(&InputSystem::HandleCursorPosition,
                                           this, std::placeholders::_1,
                                           std::placeholders::_2));

  io_service.SetMouseButtonHandler(
      std::bind(&InputSystem::HandleMouseButton, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
}

void InputSystem::OnUpdate(double dt, Scene& scene) {
  input_service.PollEvents();

  HandleInputEvents(eventQueue);

  eventQueue.clear();
}

bool InputSystem::ShouldStop(Scene&) { return input_service.ShouldStop(); }

void InputSystem::HandleCursorPosition(int x, int y) {
  InputEvent e;
  e.type = InputType::MOUSE_MOVE;
  e.mouseMove = {x, y, x - prev_cursor_pos_x, y - prev_cursor_pos_y};

  prev_cursor_pos_x = x;
  prev_cursor_pos_y = y;

  eventQueue.push_back(e);
}

void InputSystem::HandleMouseButton(int button, int action, int mods) {
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

void InputSystem::HandleKey(int key, int action, int mods) {
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

}  // namespace Bored
