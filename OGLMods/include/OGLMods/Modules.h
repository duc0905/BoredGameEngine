#pragma once
#include "../../oglpch.h"
#include "ShaderClass.h"
#include "Utils.h"

namespace Bored {
namespace OGL {
class Renderer : public Bored::Renderer {
 public:
  GLFWwindow* window;

  //std::unique_ptr<VertexArray> vao;
  //std::unique_ptr<VertexBuffer> pos_vbo;
  //std::unique_ptr<VertexBuffer> uv_vbo;
  //std::unique_ptr<VertexBuffer> norm_vbo;
  //std::unique_ptr<IndexBuffer> ibo;

  GLuint mVao, mPosVbo, mUVsVbo, mNormVbo, mIbo;

  Shader meshShader;
  Shader testShader;

  Texture white, red, mixed, img;
  std::shared_ptr<Bored::Actor> light;

  Renderer();

  // Inherited via Renderer
  virtual void OnSetup() override;
  virtual bool OnTick(double dt) override;
  virtual void OnImGui() override;
  virtual void OnStop() override;
private:
  void Render(const Bored::OGL::Model& m, Shader& shader);
  void Render(Bored::Render::Mesh&, Bored::OGL::Material&, Shader& shader);
  static void window_size_callback(GLFWwindow* window, int w, int h);
  static void framebuffer_size_callback(GLFWwindow* window, int w, int h);
};

class Input : public Bored::Input {
 public:
  GLFWwindow* window;

  // Inherited via Input
  virtual void OnSetup() override;
  virtual bool OnTick(double dt) override;
  virtual void OnStop() override;
};

};  // namespace OGL
}  // namespace Bored
