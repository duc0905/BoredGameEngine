#include "OGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

namespace Bored {
namespace Render {
namespace OGL {
Buffer::Buffer() : id(0), size(0) { glGenBuffers(1, &id); }

Buffer::~Buffer() { glDeleteBuffers(1, &id); }

VertexBuffer::VertexBuffer() : OGL::Buffer() {}
VertexBuffer::~VertexBuffer() {}

void VertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }
void VertexBuffer::Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::SubData(std::vector<char>, BufferLayout) {}

std::vector<char> VertexBuffer::GetData() { return std::vector<char>(); }

IndexBuffer::IndexBuffer() : OGL::Buffer() {}
IndexBuffer::~IndexBuffer() {}

void IndexBuffer::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
void IndexBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void IndexBuffer::SubData(std::vector<unsigned int>) {}

std::vector<char> IndexBuffer::GetData() { return std::vector<char>(); }

VertexArray::VertexArray() { glGenVertexArrays(1, &id); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id); }

void VertexArray::Bind() const { glBindVertexArray(id); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

Context::Context() {
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

Context::~Context() {}

void Context::DrawVertexArray(
    std::shared_ptr<Render::VertexArray> vao,
    std::shared_ptr<Render::ShaderPipeline> pipeline) {}

std::shared_ptr<FrameBuffer> Context::GetActiveFrameBuffer()
{
  return std::shared_ptr<FrameBuffer>();
}

bool Context::OnTick(double dt) {
  //glViewport(0, 0, window->GetWidth(), window->GetHeight());
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.2f, 0.5f, 0.7f, 1.0f);

  return true;
}

ShaderPipeline::ShaderPipeline() { id = glCreateProgram(); }

ShaderPipeline::~ShaderPipeline() { glDeleteProgram(id); }

void ShaderPipeline::Bind() { glUseProgram(id); }

void ShaderPipeline::Unbind() { glUseProgram(0); }

void ShaderPipeline::SetUniform(const std::string& name, int value) {
  // TODO
  throw std::exception("Not implemented");
}

bool ShaderPipeline::IsComplete() {
  return vShader && vShader->IsComplete() && fShader->IsComplete() &&
         (gShader ? gShader->IsComplete() : true);
}

void ShaderPipeline::LoadVertexShaderFile(std::shared_ptr<FileSystem::File> f) {
  //   LoadVertexShaderCode(f->GetData());
}

void ShaderPipeline::LoadGeometryShaderFile(
    std::shared_ptr<FileSystem::File> f) {
  //   LoadGeometryShaderCode(f->GetData());
}

void ShaderPipeline::LoadFragmentShaderFile(
    std::shared_ptr<FileSystem::File> f) {
  //   LoadFragmetShaderCode(f->GetData());
}

void ShaderPipeline::LoadVertexShaderCode(const std::string& code) {
  // TODO
  throw std::exception("Not implemented");
}

void ShaderPipeline::LoadGeometryShaderCode(const std::string& code) {
  // TODO
  throw std::exception("Not implemented");
}

void ShaderPipeline::LoadFragmentShaderCode(const std::string& code) {
  // TODO
  throw std::exception("Not implemented");
}

Texture::Texture() : id(0), width(0), height(0), bpp(0)
{
  // TODO
  glGenTextures(1, &id);
}

Texture::~Texture()
{
  //TODO
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
void* Texture2D::GetData()
{
  // TODO
  throw std::exception("Not implemented");
  return nullptr;
}

Texture2D::Texture2D() : Texture()
{
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  Unbind();
}

} // namespace OGL
}
}
