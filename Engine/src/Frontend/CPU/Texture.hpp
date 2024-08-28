#pragma once
#include "../RenderUtil.hpp"

namespace Bored
{
namespace Render
{
namespace CPU
{

class NativeTexture : public Render::NativeTexture
{
  public:
    virtual void Bind() const
    {
    }
    virtual void Unbind() const
    {
    }

    virtual void SubData(unsigned width, unsigned height, unsigned int b, void* data)
    {
        m_width = width;
        m_height = height;
        m_bpp = b;

        size_t size = m_width * m_height * m_bpp;

        m_data.clear();
        m_data.resize(size);
        memcpy(m_data.data(), data, size);
    }
    virtual std::vector<std::byte> GetData()
    {
        return m_data;
    }

    virtual void* GetId() const
    {
        return nullptr;
    }

  private:
    std::vector<std::byte> m_data;
};

class Texture : public ITexture
{
  public:
    Texture() = default;
    Texture(ITexture& other)
    {
        auto& p_tex = other.GetNativeTexture();
        m_name = other.m_name;
        m_nativeTexture.SubData(p_tex.m_width, p_tex.m_height, p_tex.m_bpp, p_tex.GetData().data());
    }

  public:
    virtual Render::NativeTexture& GetNativeTexture()
    {
        return m_nativeTexture;
    }

  private:
    NativeTexture m_nativeTexture;
};

} // namespace CPU
} // namespace Render
} // namespace Bored
