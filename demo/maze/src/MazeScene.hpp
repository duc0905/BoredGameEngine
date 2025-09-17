#pragma once

#include "Components/InputComponent.hpp"
#include "Scene/Scene.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"
#include "Utils/AssetManager.hpp"

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const std::string resource_path = "demo/maze/res/";

class PlayerController : public Bored::InputHandler {
public:
  virtual void OnInput(double dt, Bored::InputEvent &event,
                       std::shared_ptr<Bored::Node> node) override;

private:
  float speed = 10.0f;
  float sensivity = 0.8f;
  bool is_mouse_hidden = false;
};

class MazeScene : public Bored::Scene {
public:
  MazeScene();

  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;

private:
  std::shared_ptr<OGL::Renderer> renderer;
  std::shared_ptr<Bored::IOService> io;
  Bored::AssetManager &asset_manager;
};
