#pragma once

#include <Services/IOService.hpp>
#include <Components/InputComponent.hpp>
#include <Scene/Scene.hpp>
#include <Systems/Renderer/OGL/Renderer.hpp>

class CameraController : public Bored::InputHandler {
public:
  float velocity = 5.0f;
  float sensivity = 0.8f;

  bool is_mouse_hidden = false;

public:
  virtual void OnInput(double dt, Bored::InputEvent &event,
                       std::shared_ptr<Bored::Node> node) override;
};

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

class MyScene : public Bored::Scene {
public:
  MyScene();

  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;

private:
  std::shared_ptr<OGL::Renderer> renderer;
  std::shared_ptr<Bored::IOService> io;
};
