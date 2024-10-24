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

typedef std::map<std::string, ComponentType> BufferLayout;

/**
 * Helper function for BufferLayout
 */
unsigned int GetSizeOf(const ComponentType& t);

/**
 * Helper function for BufferLayout
 */
uint8_t GetCountOf(const ComponentType& t);

/**
 * Helper function for BufferLayout
 */
unsigned int GetStride(const BufferLayout& bl);

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

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual size_t GetSize() const = 0;
};

/**
 * Vertex buffer :)
 **/
class VertexBuffer : public Buffer
{
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
    virtual void SubData(const std::vector<std::byte>&) = 0;
    virtual void SubData(void* p_data, size_t size) = 0;

    virtual std::vector<std::byte> GetData() const = 0;

    void SetLayout(const BufferLayout& p_layout)
    {
        layout = p_layout;
    }

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

    virtual void SubData(std::vector<unsigned int>&) = 0;

    virtual std::vector<unsigned int> GetData() const = 0;
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

    virtual void AttachBuffer(Bored::Render::VertexBuffer& vbo) = 0;
    virtual void AttachBuffer(std::shared_ptr<VertexBuffer> vbo) = 0;
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
 * Render API's native texture object
 *
 * Provides APIs to work with native texture object of the render API
 */
class NativeTexture
{
  public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SubData(unsigned width, unsigned height, unsigned int b, void* data) = 0;
    virtual std::vector<std::byte> GetData() = 0;

    /**
     * Get the native ID of the texture
     *
     * Because ImGui needs it
     */
    virtual void* GetId() const = 0;

  public:
    unsigned int m_width, m_height, m_bpp;
};

/**
 * @brief Framebuffer holds the buffers that is generated by the renderer.
 * Has some Attachments
 **/
class FrameBuffer : public Buffer
{
  public:
    virtual std::shared_ptr<NativeTexture> GetColorTexture() = 0;
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
class ShaderProgram
{
  public:
    virtual ~ShaderProgram()
    {
    }

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetUniform(const std::string& p_name, int p_value) = 0;
    virtual void SetUniform(const std::string& p_name, int p_v1, int p_v2) = 0;
    virtual void SetUniform(const std::string& p_name, int p_v1, int p_v2, int p_v3) = 0;
    virtual void SetUniform(const std::string& p_name, int p_v1, int p_v2, int p_v3, int p_v4) = 0;

    virtual void SetUniform(const std::string& p_name, unsigned int p_value) = 0;
    virtual void SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2) = 0;
    virtual void SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2, unsigned int p_v3) = 0;
    virtual void SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2, unsigned int p_v3,
                            unsigned int p_v4) = 0;

    virtual void SetUniform(const std::string& p_name, float p_value) = 0;
    virtual void SetUniform(const std::string& p_name, float p_v1, float p_v2) = 0;
    virtual void SetUniform(const std::string& p_name, float p_v1, float p_v2, float p_v3) = 0;
    virtual void SetUniform(const std::string& p_name, float p_v1, float p_v2, float p_v3, float p_v4) = 0;

    virtual void SetUniform(const std::string& p_name, glm::mat2 p_value) = 0;
    virtual void SetUniform(const std::string& p_name, glm::mat3 p_value) = 0;
    virtual void SetUniform(const std::string& p_name, glm::mat4 p_value) = 0;

    virtual void LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f) = 0;
    virtual void LoadGeometryShaderFile(std::shared_ptr<FileSystem::File> f) = 0;
    virtual void LoadFragmentShaderFile(std::shared_ptr<FileSystem::File> f) = 0;

    virtual void LoadVertexShaderCode(const std::string& code) = 0;
    virtual void LoadGeometryShaderCode(const std::string& code) = 0;
    virtual void LoadFragmentShaderCode(const std::string& code) = 0;

    virtual void Link() = 0;
    virtual bool IsLinked() = 0;

    // public:
    //   virtual void Draw(std::shared_ptr<Render::IMesh> p_mesh, std::shared_ptr<Render::Material> p_material) = 0;

  protected:
    std::unique_ptr<VertexShader> vShader;
    std::unique_ptr<GeometryShader> gShader;
    std::unique_ptr<FragmentShader> fShader;
};

/**
 * TODO: write this
 *
 * Abstract Factory pattern to create render utility for drivers
 *
 * @param name Type and description of the parameter.
 * @return Type and description of the returned value.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
class Context
{
  public:
    virtual ~Context()
    {
    }

    // virtual void DrawVertexArray(std::shared_ptr<VertexArray> vao, std::shared_ptr<ShaderProgram> pipeline) = 0;

    virtual FrameBuffer& GetActiveFrameBuffer() = 0;
    virtual void ClearFrameBuffer(const glm::vec4&) = 0;
    virtual void SetViewport(int l, int b, int r, int t) = 0;

    virtual void Draw(ShaderProgram& p_shader, VertexArray& p_vao, IndexBuffer& p_ebo) = 0;
};
} // namespace Render
} // namespace Bored
