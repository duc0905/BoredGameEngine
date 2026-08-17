#pragma once

#include <Components/InputComponent.hpp>
#include <Scene/Scene.hpp>
#include <Systems/Renderer/OGL/Renderer.hpp>

#include "Components/BehaviourComponent.hpp"
#include "Systems/Input/InputSystem.hpp"

class CameraController : public Bored::Behaviour {
 public:
  CameraController() : Bored::Behaviour() {}

  float velocity = 5.0f;
  float sensivity = 0.8f;

  bool is_mouse_hidden = false;

  int dx = 0, dy = 0;

  bool move_forward = false;
  bool move_backward = false;
  bool move_left = false;
  bool move_right = false;

 public:
  virtual void OnUpdate(float dt) override;

  void MoveCamera(Bored::InputEvent& e);

  void RotateCamera(Bored::InputEvent& e);
};

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

/*
 * @todo Create a simple InputSystem for this demo.
 */
class MyScene : public Bored::Scene {
 public:
  MyScene();

  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;

 private:
  std::shared_ptr<OGL::Renderer> renderer;

  std::shared_ptr<Bored::InputSystem> input;
};
