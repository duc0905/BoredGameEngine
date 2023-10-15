#pragma once
#include "../Utils/Render.hpp"

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

    std::shared_ptr<Render::Texture> GetMainColorTexture()
    {
        return context->GetActiveFrameBuffer()->GetColorTexture();
    }

  private:
    std::unique_ptr<Render::Context> context;
};
}
}