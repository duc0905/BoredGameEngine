#pragma once
#include "../../Window/Listeners.hpp"
#include "../I_Renderer.hpp"

/**
 * Render using per-pixel ray tracing on the CPU.
 */
class CPU_RayTracer : public I_Renderer3D, public FrameBufferSizeListener {
public:
  CPU_RayTracer(Camera &&camera, int width, int height);

  virtual void SetupObjects(
      const std::vector<std::shared_ptr<I_Object3D>> &objects) override;

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::unique_ptr<I_Texture2D> Render() override;

  /**
   * Inherits from FrameBufferSizeListener
   */
  virtual void OnFrameBufferSize(int width, int height) override;

private: // Ray tracing stuff
  [[nodiscard]] glm::vec3 TraceRay(const Ray3D &ray, int depth,
                                   std::shared_ptr<I_Object3D> Os);

private:
  // Params to render
  float m_du, m_dv;
  float m_sx, m_sy;
  float m_left_c, m_bottom_c;
  glm::vec3 m_bgColor;

  int m_width, m_height; /**< size of the framebuffer */

private:
  std::vector<std::shared_ptr<I_RTObject>> m_objs;
};
