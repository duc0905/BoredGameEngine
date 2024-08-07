#pragma once

#include <glad/glad.h>
#include "Render.h"
// #include "Window.h"
#include <vector>

namespace Bored
{
namespace Render
{
namespace OGL
{
class Buffer : public Render::Buffer
{
  public:
    Buffer();
    virtual ~Buffer();

  protected:
    GLuint id;
    // Size in bytes
    unsigned int size;
};

class VertexBuffer : public Buffer, public Render::VertexBuffer
{
  public:
    VertexBuffer();
    virtual ~VertexBuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void SubData(std::vector<char>, BufferLayout) override;
};

class IndexBuffer : public Buffer, public Render::IndexBuffer
{
  public:
    IndexBuffer();
    virtual ~IndexBuffer();

    virtual void Bind() override;
    virtual void Unbind() override;

    // Inherited via Buffer
    void SubData(std::vector<unsigned int>) override;
};

class VertexArray : public Render::VertexArray
{
  public:
    VertexArray();
    virtual ~VertexArray();
    virtual void Bind() const override;
    virtual void Unbind() const override;

    // TODO provide a way to attach vertex buffer
  private:
    GLuint id;
    std::vector<Buffer> vertexBuffers;
};

class ShaderPipeline : Render::ShaderPipeline
{
  public:
    ShaderPipeline();
    ~ShaderPipeline();

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

  private:
    GLuint id;
};

class Texture : public Render::Texture
{
  public:
    Texture();
    ~Texture();
    void* GetId() const override;

  protected:
    GLuint id;
};

class Texture2D : public Texture
{
  public:
    Texture2D();
    ~Texture2D();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SubData(unsigned width, unsigned height, unsigned int b, void* data) override;
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

    void Bind() override;
    void Unbind() override;
    virtual std::shared_ptr<Render::Texture> GetColorTexture() override;
    virtual bool CheckStatus() override;
    virtual bool HasDepthTest() override;
    virtual bool HasStencilTest() override;
    virtual void AddRenderBuffer(std::shared_ptr<RenderBuffer> rbo) override;

    void ResizeBuffers(int w, int h);

    static FrameBuffer* GetDefault();

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

    virtual void DrawVertexArray(std::shared_ptr<Render::VertexArray> vao,
                                 std::shared_ptr<Render::ShaderPipeline> pipeline) override;

    Render::FrameBuffer& GetActiveFrameBuffer() override;

    virtual void ClearFrameBuffer(const glm::vec4&) override;
    virtual void SetViewport(int l, int b, int r, int t) override;

    virtual std::shared_ptr<Render::Texture> CreateTexture() override;

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
