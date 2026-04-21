#pragma once

#include "../../../Components/Mesh3D.hpp"
#include "../../../Scene/Scene.hpp"
#include "../I_Renderer.hpp"
#include "../Shader/Shader.hpp"
#include "../Texture/OGL_Texture.hpp"
#include <memory>


namespace OGL {
class Renderer : public Bored::I_Renderer3D {
public:
  Renderer(Bored::IOService &io_service);

  virtual ~Renderer();

  /**
   * This function is called once before any Render call.
   *
   * A renderer can implement this function to setup render data for the objects
   * and implement any pre-rendering optimizations.
   */
  virtual void SetupObjects(Bored::Scene &scene) override;

  /**
   * Interface function responsible for rendering the scene.
   */
  virtual std::shared_ptr<I_Texture2D> Render(Bored::Scene &scene) override;

  virtual void OnUpdate(double dt, Bored::Scene &scene) override {
    auto texture = Render(scene);
    scene.context.io->Render(texture);
  }

  void OnFrameBufferSize(int width, int height);

private:
  void ResizeColorBuffer(int width, int height);
  void ResizeDepthBuffer(int width, int height);

private:
  int m_width, m_height;

  GLuint m_fbo, m_depthBuffer;
  std::shared_ptr<OGL_Texture2D> m_colorTexture;

  Bored::IOService &io;

public:
  static std::shared_ptr<Shader> defaultMeshShader;

  static std::shared_ptr<::Bored::Material> defaultMaterial;
};

} // namespace OGL
