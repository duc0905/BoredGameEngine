#pragma once

#include "Components/InputComponent.hpp"
#include "GravitySystem.hpp"
#include "Scene/Scene.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const std::string resource_path = "demo/maze/res/";

class PlayerController : public Bored::InputHandler {
public:
  virtual void OnInput(double dt, Bored::InputEvent &event,
                       std::shared_ptr<Bored::Node> node) override;

  void UpdateMovement(Bored::TransformComponent& trans, PhysicsComponent& phys);
private:
  // up, down, right, left, space, ctrl
  bool keydown[6]{false, false, false, false, false, false};

  float speed = 1.5f;
  float jump_strength = 4.0f;
  float sensivity = 0.8f;
  bool is_mouse_hidden = false;
};

class MazeScene : public Bored::Scene {
public:
  MazeScene(const std::string& filepath);

  /**
   * Construct the maze from the file given in the constructor.
   */
  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;

private:
  unsigned int width, height;
  std::vector<std::vector<char>> level;

  std::shared_ptr<OGL::Renderer> renderer;
  std::shared_ptr<Bored::IOService> io;
};
