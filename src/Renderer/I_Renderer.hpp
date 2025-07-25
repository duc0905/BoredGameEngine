#pragma once
#include "../ECS/Scene.hpp"
#include "I_Object.hpp"
#include "I_Texture.hpp"
#include <vector>

/**
 * Interface for a Renderer.
 */
class I_Renderer3D {
public:
  virtual ~I_Renderer3D() {}

  /**
   * This function is called once before any Render call.
   *
   * A renderer can implement this function to setup render data for the objects
   * and implement any pre-rendering optimizations.
   */
  virtual void SetupObjects(std::shared_ptr<Bored::Scene> scene) {}

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::shared_ptr<I_Texture2D> Render() = 0;


  void SetActiveScene(std::shared_ptr<Bored::Scene> scene) { active_scene = scene; }

public:
  glm::vec4 m_bg = {0.1f, 0.2f, 0.05f, 1.0f};

protected:
  std::shared_ptr<Bored::Scene> active_scene;
};
