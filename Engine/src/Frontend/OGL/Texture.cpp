#include "Texture.hpp"

namespace Bored
{
namespace Render
{
namespace OGL
{

Texture::Texture()
{
}

Texture::Texture(ITexture& other)
{
    auto& p_tex = other.GetNativeTexture();
    m_name = other.m_name;
    m_nativeTexture.SubData(p_tex.m_width, p_tex.m_height, p_tex.m_bpp, p_tex.GetData().data());
}

} // namespace OGL
} // namespace Render
} // namespace Bored
