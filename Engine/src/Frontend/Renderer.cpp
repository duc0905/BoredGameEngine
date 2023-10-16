#include "Renderer.hpp"

namespace Bored
{
namespace Frontend
{
Renderer::Renderer(Render::Context* c) : context(c) {
}

std::shared_ptr<Render::Texture> Renderer::GetMainColorTexture()
{
  return context->GetActiveFrameBuffer().GetColorTexture();
}

void Renderer::BindFramebuffer()
{
  context->GetActiveFrameBuffer().Bind();
}

void Renderer::SetViewport(int l, int b, int r, int t)
{
  context->SetViewport(l, b, r, t);
}

void Renderer::SetClearColor(const glm::vec4& c)
{
  clearColor = c;
}

void Renderer::SetClearColor(const glm::vec3& c)
{
  SetClearColor(glm::vec4(c, 1.0f));
}

void Renderer::Clear()
{
  context->ClearFrameBuffer(clearColor);
}
}
}