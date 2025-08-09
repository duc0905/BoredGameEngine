#pragma once

#include "Systems/Input/IOService.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"
#include "Scene/Scene.hpp"

class MyScene : public Bored::Scene {
public:
  MyScene();

  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;

private:
  std::shared_ptr<OGL::Renderer> renderer;
  std::shared_ptr<Bored::IOService> io;
};
