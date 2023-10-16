#include "OGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace Bored
{
namespace Render
{
namespace OGL
{
Buffer::Buffer() : id(0), size(0)
{
    glGenBuffers(1, &id);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &id);
}

VertexBuffer::VertexBuffer() : OGL::Buffer()
{
}
VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SubData(std::vector<char>, BufferLayout)
{
}

IndexBuffer::IndexBuffer() : OGL::Buffer()
{
}
IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SubData(std::vector<unsigned int>)
{
}

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

Texture::Texture() : id(0), width(0), height(0), bpp(0)
{
    glGenTextures(1, &id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void* Texture::GetId() const
{
    return (void*)(intptr_t)id;
}

unsigned int Texture::GetWidth() const
{
    return width;
}

unsigned int Texture::GetHeight() const
{
    return height;
}

unsigned int Texture::GetBPP() const
{
    return bpp;
}

void Texture2D::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture2D::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::SubData(unsigned w, unsigned h, unsigned int b, void* d)
{
    width = w;
    height = h;
    bpp = b;

    // TODO test
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, d);
    Unbind();
}

Texture2D::Texture2D() : Texture()
{
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Unbind();
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id);
}

FrameBuffer::FrameBuffer(int w, int h)
: colorBuffer(std::make_shared<Texture2D>())
{
    glGenFramebuffers(1, &id);
    Bind();
    colorBuffer->SubData(w, h, 3, nullptr);
    colorBuffer->Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (GLuint)(intptr_t)colorBuffer->GetId(), 0);
    colorBuffer->Unbind();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Render::Texture> FrameBuffer::GetColorTexture()
{
    return colorBuffer;
}

bool FrameBuffer::CheckStatus()
{
    Bind();
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool FrameBuffer::HasDepthTest()
{
    throw std::exception("Not implemented");
    return false;
}

bool FrameBuffer::HasStencilTest()
{
    throw std::exception("Not implemented");
    return false;
}

void FrameBuffer::AddRenderBuffer(std::shared_ptr<RenderBuffer> rbo)
{
    throw std::exception("Not implemented");
}

void FrameBuffer::ResizeBuffers(int w, int h)
{
    if (id != 0)
        colorBuffer->SubData(w, h, 4, nullptr);
}

ShaderPipeline::ShaderPipeline()
{
    id = glCreateProgram();
}

ShaderPipeline::~ShaderPipeline()
{
    glDeleteProgram(id);
}

void ShaderPipeline::Bind()
{
    glUseProgram(id);
}

void ShaderPipeline::Unbind()
{
    glUseProgram(0);
}

void ShaderPipeline::SetUniform(const std::string& name, int value)
{
    throw std::exception("Not implemented");
}

bool ShaderPipeline::IsComplete()
{
    return vShader && vShader->IsComplete() && fShader->IsComplete() && (gShader ? gShader->IsComplete() : true);
}

void ShaderPipeline::LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f)
{
    //   LoadVertexShaderCode(f->GetData());
    throw std::exception("Not implemented");
}

void ShaderPipeline::LoadGeometryShaderFile(std::shared_ptr<FileSystem::File> f)
{
    //   LoadGeometryShaderCode(f->GetData());
    throw std::exception("Not implemented");
}

void ShaderPipeline::LoadFragmentShaderFile(std::shared_ptr<FileSystem::File> f)
{
    //   LoadFragmetShaderCode(f->GetData());
    throw std::exception("Not implemented");
}

void ShaderPipeline::LoadVertexShaderCode(const std::string& code)
{
    throw std::exception("Not implemented");
}

void ShaderPipeline::LoadGeometryShaderCode(const std::string& code)
{
    throw std::exception("Not implemented");
}

void ShaderPipeline::LoadFragmentShaderCode(const std::string& code)
{
    throw std::exception("Not implemented");
}

Context::Context()
{
    fbo = std::make_unique<FrameBuffer>();
}

Context::~Context() {}

void Context::DrawVertexArray(std::shared_ptr<Render::VertexArray> vao,
                              std::shared_ptr<Render::ShaderPipeline> pipeline)
{
}

Render::FrameBuffer& Context::GetActiveFrameBuffer()
{
    return *fbo;
}

void Context::ClearFrameBuffer(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Context::SetViewport(int l, int b, int r, int t)
{
    fbo->Bind();
    glViewport(l, b, r, t);
    int newWidth = abs(r - l);
    int newHeight = abs(t - b);

    if (newWidth != width && newHeight != height)
    {
        width = newWidth;
        height = newHeight;
        fbo->ResizeBuffers(width, height);
    }
}

FrameBuffer* FrameBuffer::defaultFbo = nullptr;
FrameBuffer::FrameBuffer(int x)
    : id(0) {}

FrameBuffer* FrameBuffer::GetDefault()
{
    if (!defaultFbo)
        defaultFbo = new FrameBuffer(0);
    return defaultFbo;
}

Context* Context::defaultContext = nullptr;
Context::Context(int x) {
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    fbo = std::unique_ptr<FrameBuffer>(FrameBuffer::GetDefault());
}

Context* Context::GetDefault()
{
    if (!defaultContext)
        defaultContext = new Context(0);
    return defaultContext;
}
} // namespace OGL
} // namespace Render
} // namespace Bored
