#pragma once
#include "../Utils/Render.hpp"

namespace Bored
{
namespace Frontend
{
class Renderer
{
  public:
    std::shared_ptr<Render::Texture> GetMainColorTexture()
    {
        return context->GetActiveFrameBuffer()->GetColorTexture();
    }

  private:
    std::shared_ptr<Render::Context> context;
};
}
}