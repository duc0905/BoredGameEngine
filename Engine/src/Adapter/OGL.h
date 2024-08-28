#pragma once

#include <glad/glad.h>
#include "Render.h"
#include <cstddef>
#include <map>
#include <vector>

namespace Bored
{
namespace Render
{
namespace OGL
{

// class Buffer : public Render::Buffer
// {
//   public:
//     Buffer();
//     virtual ~Buffer();

//   protected:
//     GLuint id;
//     // Size in bytes
//     unsigned int size;
// };

class VertexBuffer : public Render::VertexBuffer
{
  public:
    VertexBuffer();
    virtual ~VertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SubData(const std::vector<std::byte>&) override;
    virtual void SubData(void* p_data, size_t size) override;
    virtual std::vector<std::byte> GetData() const override;
    virtual size_t GetSize() const override
    {
        return size;
    };

  private:
    GLuint id;
    unsigned int size = 0;
};

class IndexBuffer : public Render::IndexBuffer
{
  public:
    IndexBuffer();
    virtual ~IndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    // Inherited via Buffer
    virtual void SubData(std::vector<unsigned int>&) override;
    virtual std::vector<unsigned int> GetData() const override;
    virtual size_t GetSize() const override
    {
        return size;
    };

  private:
    GLuint id;
    size_t size = 0;
};

class VertexArray : public Render::VertexArray
{
  public:
    VertexArray();
    virtual ~VertexArray();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AttachBuffer(Bored::Render::VertexBuffer& vbo) override;
    virtual void AttachBuffer(std::shared_ptr<Bored::Render::VertexBuffer> vbo) override;

  private:
    GLuint id;
    unsigned int m_attribIndex{};
    // std::vector<VertexBuffer> vertexBuffers;
};

class NativeTexture : public Render::NativeTexture
{
  public:
    NativeTexture();
    ~NativeTexture();
    void* GetId() const override;

  protected:
    GLuint id;
};

class Texture2D : public NativeTexture
{
  public:
    Texture2D();
    ~Texture2D();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SubData(unsigned width, unsigned height, unsigned int b, void* data) override;
    virtual std::vector<std::byte> GetData() override;
};

// TODO: implement
class VertexShader : public Render::VertexShader
{
  public:
    virtual ~VertexShader()
    {
    }
};

// TODO: implement
class GeometryShader : public Render::GeometryShader
{
  public:
    virtual ~GeometryShader()
    {
    }
};

// TODO: implement
class FragmentShader : public Render::FragmentShader
{
  public:
    virtual ~FragmentShader()
    {
    }
};

// TODO: implement
class ShaderProgram : public Render::ShaderProgram
{
  public:
    ShaderProgram();
    virtual ~ShaderProgram();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void SetUniform(const std::string& p_name, int p_value) override;
    virtual void SetUniform(const std::string& p_name, int p_v1, int p_v2) override;
    virtual void SetUniform(const std::string& p_name, int p_v1, int p_v2, int p_v3) override;
    virtual void SetUniform(const std::string& p_name, int p_v1, int p_v2, int p_v3, int p_v4) override;

    virtual void SetUniform(const std::string& p_name, unsigned int p_value) override;
    virtual void SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2) override;
    virtual void SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2, unsigned int p_v3) override;
    virtual void SetUniform(const std::string& p_name, unsigned int p_v1, unsigned int p_v2, unsigned int p_v3, unsigned int p_v4) override;

    virtual void SetUniform(const std::string& p_name, float p_value) override;
    virtual void SetUniform(const std::string& p_name, float p_v1, float p_v2) override;
    virtual void SetUniform(const std::string& p_name, float p_v1, float p_v2, float p_v3) override;
    virtual void SetUniform(const std::string& p_name, float p_v1, float p_v2, float p_v3, float p_v4) override;

    virtual void SetUniform(const std::string& p_name, glm::mat2 p_value) override;
    virtual void SetUniform(const std::string& p_name, glm::mat3 p_value) override;
    virtual void SetUniform(const std::string& p_name, glm::mat4 p_value) override;

    virtual void LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f) override;
    virtual void LoadGeometryShaderFile(std::shared_ptr<FileSystem::File> f) override;
    virtual void LoadFragmentShaderFile(std::shared_ptr<FileSystem::File> f) override;

    virtual void LoadVertexShaderCode(const std::string& code) override;
    virtual void LoadGeometryShaderCode(const std::string& code) override;
    virtual void LoadFragmentShaderCode(const std::string& code) override;

    virtual void Link() override;
    virtual bool IsLinked() override;

private:
    GLint GetLocation(const std::string& p_name);
  private:
    GLuint m_id;
    GLint m_isLinked = GL_FALSE;
    std::map<std::string, GLint> m_locations;
};

/**
 * @brief OpenGL implementation of Render::FrameBuffer
 */
class FrameBuffer : public Render::FrameBuffer
{
  public:
    FrameBuffer(int w, int h);
    FrameBuffer() : FrameBuffer(0, 0)
    {
    }
    ~FrameBuffer();

    void Bind() const override;
    void Unbind() const override;
    virtual std::shared_ptr<Render::NativeTexture> GetColorTexture() override;
    virtual bool CheckStatus() override;
    virtual bool HasDepthTest() override;
    virtual bool HasStencilTest() override;
    virtual void AddRenderBuffer(std::shared_ptr<RenderBuffer> rbo) override;

    void ResizeBuffers(int w, int h);

    static FrameBuffer* GetDefault();
    virtual size_t GetSize() const override
    {
        // TODO:
        return 0;
    };

  private:
    GLuint id;
    std::shared_ptr<Texture2D> colorBuffer;

    // Special constructor used to create defaultFbo
    FrameBuffer(int x);
    // Should not read from default FBO since it is rendered to the screen
    static FrameBuffer* defaultFbo;
};

class Context : public Render::Context
{
  public:
    Context();
    virtual ~Context();

    Render::FrameBuffer& GetActiveFrameBuffer() override;

    virtual void ClearFrameBuffer(const glm::vec4&) override;
    virtual void SetViewport(int l, int b, int r, int t) override;

    virtual void Draw(Render::ShaderProgram& p_shader, Render::VertexArray& p_vao, Render::IndexBuffer& p_ebo) override;

    static Context* GetDefault();

  private:
    std::unique_ptr<FrameBuffer> fbo;
    int width = 0, height = 0;

    // Special constructor only used to create the default context
    Context(int x);
    static Context* defaultContext;
};
} // namespace OGL
} // namespace Render
} // namespace Bored
