#pragma once
#include "../RenderUtil.hpp"
#include "../../Adapter/OGL.h"

namespace Bored
{
namespace Render
{
namespace OGL
{

class Texture : public ITexture
{
  public:
    Texture();
    Texture(ITexture& other);

    virtual Render::NativeTexture& GetNativeTexture() override { return m_nativeTexture; }
  private:
    OGL::Texture2D m_nativeTexture;
};

}
} // namespace Render
} // namespace Bored
