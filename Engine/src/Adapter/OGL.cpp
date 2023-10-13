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

Context::Context(Bored::Window::Window* w) : window(w) {
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

Context::~Context() {}

void Context::DrawVertexArray(
    std::shared_ptr<Render::VertexArray> vao,
    std::shared_ptr<Render::ShaderPipeline> pipeline) {}

Bored::Window::Window& Context::GetWindow() const
{
  return *window;
}

std::shared_ptr<FrameBuffer> Context::GetActiveFrameBuffer()
{
  return std::shared_ptr<FrameBuffer>();
}

bool Context::OnTick(double dt) {
  glViewport(0, 0, window->GetWidth(), window->GetHeight());
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

void Texture::Bind() const
{
}

void Texture::Unbind() const
{
}

void Texture::SubData(unsigned w, unsigned h, unsigned int b, void* d)
{
  width = w;
  height = h;
  bpp = b;
  data = (char*)d;
  // TODO call opengl api
}
} // namespace OGL
}
}
