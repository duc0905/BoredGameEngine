#pragma once
#include "../../Scene/Scene.hpp"
#include "../I_System.hpp"
#include "I_Texture.hpp"

namespace Bored {
/**
 * Interface for a Renderer.
 */
class I_Renderer3D : public I_System {
public:
  virtual ~I_Renderer3D() {}

  /**
   * This function is called once before any Render call.
   *
   * A renderer can implement this function to setup render data for the objects
   * and implement any pre-rendering optimizations.
   */
  virtual void SetupObjects(Scene& scene) {}

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::shared_ptr<I_Texture2D> Render(Scene &scene) = 0;
public:
  glm::vec4 m_bg = {0.1f, 0.2f, 0.05f, 1.0f};
};

} // namespace Bored
