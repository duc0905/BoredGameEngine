#include "RenderManager.hpp"
#include <memory>

#if RENDER_API == OGL_API
#include "../Adapter/OGL.cpp"  // Compile OGL.cpp only if the render
#endif

#include "../Adapter/Render.h"
#include "../Adapter/Window.h"

namespace Bored {
namespace Render {
std::unique_ptr<Manager> Manager::instance_ = nullptr;

Manager::Manager() : isInit(false), context(nullptr) {}

Manager& Manager::GetInstance() {
  if (!instance_) instance_ = std::unique_ptr<Manager>(new Manager());
  return *instance_;
}

void Manager::UseRenderContext(Bored::Window::Window* w) {
  context = w->GetRenderContext();
}

std::shared_ptr<VertexArray> Manager::CreateVAO() {
#if RENDER_API == OGL_API
  return std::make_shared<OGL::VertexArray>();
#else
  return nullptr;
#endif
}

std::shared_ptr<VertexBuffer> Manager::CreateVBO() {
#if RENDER_API == OGL_API
  return std::make_shared<OGL::VertexBuffer>();
#else
  return nullptr;
#endif
}

std::shared_ptr<IndexBuffer> Manager::CreateIBO() {
#if RENDER_API == OGL_API
  return std::make_shared<OGL::IndexBuffer>();
#else
  return nullptr;
#endif
}

std::shared_ptr<ColorBuffer> Manager::CreateCBO() { return nullptr; }
std::shared_ptr<RenderBuffer> Manager::CreateRBO() { return nullptr; }
std::shared_ptr<DepthBuffer> Manager::CreateDBO() { return nullptr; }
std::shared_ptr<StencilBuffer> Manager::CreateSBO() { return nullptr; }
std::shared_ptr<FrameBuffer> Manager::CreateFBO() { return nullptr; }
std::shared_ptr<Texture2D> Manager::CreateTexture2D() { return nullptr; }
std::shared_ptr<ShaderPipeline> Manager::CreateShaderProgram() {
  return nullptr;
}
}  // namespace Render
}  // namespace Bored
