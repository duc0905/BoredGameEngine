#pragma once

#include "../../Nodes/Mesh3D.hpp"
#include "../../Window/Listeners.hpp"
#include "../I_Object.hpp"
#include "../I_Renderer.hpp"
#include "../Shader/Shader.hpp"
#include "../Texture/OGL_Texture.hpp"
#include <memory>

namespace OGL {
class Renderer : public I_Renderer3D, public FrameBufferSizeListener {
public:
  Renderer(int width, int height);

  virtual ~Renderer();

  /**
   * This function is called once before any Render call.
   *
   * A renderer can implement this function to setup render data for the objects
   * and implement any pre-rendering optimizations.
   */
  virtual void
  SetupObjects(const std::vector<std::shared_ptr<I_Object3D>> &objects) override;

  virtual void
  SetupObjects(std::shared_ptr<Bored::Scene> scene) override;

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::shared_ptr<I_Texture2D> Render() override;

  /**
   * Inherits from FrameBufferSizeListener
   */
  virtual void OnFrameBufferSize(int width, int height) override;
public:
  void SetBackgroundColor(const glm::vec4& color);

  void SetActiveScene(std::shared_ptr<Bored::Scene> scene);
private:
  void ResizeColorBuffer(int width, int height);
  void ResizeDepthBuffer(int width, int height);

private:
  glm::vec4 m_bg = {0.1f, 0.2f, 0.05f, 1.0f};

  int m_width, m_height;

  std::shared_ptr<Bored::Scene> active_scene;

  GLuint m_fbo, m_depthBuffer;
  std::shared_ptr<OGL_Texture2D> m_colorTexture;

public:
  static std::shared_ptr<Shader> defaultMeshShader;
};

// TODO: Move this to a asset manager
std::shared_ptr<Bored::ArrayMesh> LoadModel(const std::string& filepath);
} // namespace OGL
