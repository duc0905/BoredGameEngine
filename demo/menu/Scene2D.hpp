#pragma once

#include "Scene/Scene.hpp"

class Scene2D : public Bored::Scene {
public:
  Scene2D();

  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;
};
