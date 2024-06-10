#pragma once
#include <glm/glm.hpp>
#include "../Adapter/Render.h"

namespace Bored
{
namespace Frontend
{
/**
 * @brief Use the context to create render outputs
 * 
*/
class Renderer
{
  public:
    Renderer(Render::Context* c);

    std::shared_ptr<Render::Texture> GetMainColorTexture();

    void BindFramebuffer();

    void SetViewport(int l, int b, int r, int t);
    void SetClearColor(const glm::vec4& c);
    void SetClearColor(const glm::vec3& c);
    void Clear();

  private:
    Render::Context* context;
    glm::vec4 clearColor;
};
}
}
