#pragma once

#include <glad/glad.h>
#include "Render.h"
#include "Window.h"
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
    std::vector<char> GetData() override;
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
    std::vector<char> GetData() override;
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

class ColorBuffer : public Render::ColorBuffer
{
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

    unsigned int GetWidth() const override;
    unsigned int GetHeight() const override;
    unsigned int GetBPP() const override;
  protected:
    GLuint id;
    unsigned int width, height, bpp;
};

class Texture2D : public Texture
{
  public:
    Texture2D();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SubData(unsigned width, unsigned height, unsigned int b, void* data) override;
    virtual void* GetData() override;
};

class Context : public Render::Context
{
  public:
    Context();
    virtual ~Context();

    virtual bool OnTick(double dt) override;

    virtual void DrawVertexArray(std::shared_ptr<Render::VertexArray> vao,
                                 std::shared_ptr<Render::ShaderPipeline> pipeline) override;

    std::shared_ptr<FrameBuffer> GetActiveFrameBuffer() override;
};
} // namespace OGL
} // namespace Render
} // namespace Bored
