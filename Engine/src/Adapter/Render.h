#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "FileSystem.h"

namespace Bored
{
namespace Render
{
enum ComponentType
{
    None = 0,
    Bool,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Mat3,
    Mat4
};

/**
 * A buffer in the GPU
 * Will not store data in the GPU, so will not store data in here
 **/
class Buffer
{
  public:
    virtual ~Buffer()
    {
    }

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
};

/**
 * Vertex buffer :)
 **/
class VertexBuffer : public Buffer
{
  public:
    typedef std::map<std::string, ComponentType> BufferLayout;

  public:
    virtual ~VertexBuffer()
    {
    }
    /* *
     * Get buffer's layout
     * */
    BufferLayout GetLayout() const
    {
        return layout;
    }

    /* *
     * Substitute data inside the buffer with new data
     * */
    virtual void SubData(std::vector<char>, BufferLayout) = 0;

  protected:
    /* *
     * Layout of the data inside the buffer
     * */
    BufferLayout layout;
};

/**
 * Buffer of indices to render
 **/
class IndexBuffer : public Buffer
{
  public:
    virtual ~IndexBuffer()
    {
    }

    virtual void SubData(std::vector<unsigned int>) = 0;
};

/* *
 * Vertex Array :)
 * Always have an index buffer by default
 * */
class VertexArray
{
  public:
    virtual ~VertexArray()
    {
    }

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

  protected:
    std::unique_ptr<IndexBuffer> ibo;
};

class ITexture
{
  public:
    virtual void* GetId() const = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SubData(unsigned width, unsigned height, unsigned int bpp, void* data) = 0;

  public:
    std::string _name;
    unsigned int _width;
    unsigned int _height;
    unsigned int _bpp;
};

class CPUTexture : public ITexture
{
  public:
    virtual void* GetId() const
    {
        return nullptr;
    }
    virtual void Bind() const
    {
    }
    virtual void Unbind() const
    {
    }
    virtual void SubData(unsigned width, unsigned height, unsigned int bpp, void* data)
    {
    }
};

class OGLTexture : public ITexture
{
  public:
    virtual void* GetId() const
    {
        return nullptr;
    }
    virtual void Bind() const
    {
    }
    virtual void Unbind() const
    {
    }
    virtual void SubData(unsigned width, unsigned height, unsigned int bpp, void* data)
    {
    }
};

class Texture2D : private ITexture
{
};
class Texture3D : private ITexture
{
};
class Cubemap : private ITexture
{
};

class Attachment
{
  public:
    virtual bool CheckStatus() = 0;
};

/**
 * TODO
 **/
class RenderBuffer : public Attachment
{
};

/**
 * @brief Framebuffer holds the buffers that is generated by the renderer.
 * Has some Attachments
 **/
class FrameBuffer : public Buffer
{
  public:
    virtual std::shared_ptr<ITexture> GetColorTexture() = 0;
    virtual bool CheckStatus() = 0;
    virtual bool HasDepthTest() = 0;
    virtual bool HasStencilTest() = 0;
    virtual void AddRenderBuffer(std::shared_ptr<RenderBuffer> rbo) = 0;
};

class Shader
{
  public:
    virtual bool IsComplete() = 0;
};
class VertexShader : public Shader
{
  public:
    virtual ~VertexShader()
    {
    }
};
class GeometryShader : public Shader
{
  public:
    virtual ~GeometryShader()
    {
    }
};
class FragmentShader : public Shader
{
  public:
    virtual ~FragmentShader()
    {
    }
};

/**
 * Also known as Shader program
 **/
class ShaderPipeline
{
  public:
    virtual ~ShaderPipeline()
    {
    }

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void SetUniform(const std::string& name, int value) = 0;
    virtual bool IsComplete() = 0;

    virtual void LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f) = 0;
    virtual void LoadGeometryShaderFile(std::shared_ptr<FileSystem::File> f) = 0;
    virtual void LoadFragmentShaderFile(std::shared_ptr<FileSystem::File> f) = 0;

    virtual void LoadVertexShaderCode(const std::string& code) = 0;
    virtual void LoadGeometryShaderCode(const std::string& code) = 0;
    virtual void LoadFragmentShaderCode(const std::string& code) = 0;

  protected:
    std::unique_ptr<VertexShader> vShader;
    std::unique_ptr<GeometryShader> gShader;
    std::unique_ptr<FragmentShader> fShader;
};

class Context
{
  public:
    virtual ~Context()
    {
    }

    virtual void DrawVertexArray(std::shared_ptr<VertexArray> vao, std::shared_ptr<ShaderPipeline> pipeline) = 0;
    virtual FrameBuffer& GetActiveFrameBuffer() = 0;
    virtual void ClearFrameBuffer(const glm::vec4&) = 0;
    virtual void SetViewport(int l, int b, int r, int t) = 0;

    virtual std::shared_ptr<ITexture> CreateTexture() = 0;
};
} // namespace Render
} // namespace Bored
