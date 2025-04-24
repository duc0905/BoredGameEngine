#pragma once
#include "Camera/Camera.hpp"
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
  virtual void
  SetupObjects(const std::vector<std::shared_ptr<I_Object3D>> &objects) {}

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::unique_ptr<I_Texture2D> Render() = 0;

public:
  Camera m_camera; /**< The camera from which to render the scene */
};
