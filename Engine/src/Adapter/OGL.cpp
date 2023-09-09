#include "OGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Bored {
  namespace Render {
    namespace OGL {
      Buffer::Buffer() : id(0), size(0) {
        glGenBuffers(1, &id);
      }

      Buffer::~Buffer() {
        glDeleteBuffers(1, &id);
      }

      VertexBuffer::VertexBuffer() : OGL::Buffer() {}
      VertexBuffer::~VertexBuffer() {}

      void VertexBuffer::Bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
      }
      void VertexBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
      }

      void VertexBuffer::SubData(std::vector<char>, BufferLayout)
      {
      }

      std::vector<char> VertexBuffer::GetData()
      {
        return std::vector<char>();
      }

      IndexBuffer::IndexBuffer() : OGL::Buffer() {}
      IndexBuffer::~IndexBuffer() { }

      void IndexBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
      }
      void IndexBuffer::Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }

      void IndexBuffer::SubData(std::vector<unsigned int>)
      {
      }

      std::vector<char> IndexBuffer::GetData()
      {
        return std::vector<char>();
      }

      VertexArray::VertexArray() {
        glGenVertexArrays(1, &id);
      }

      VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &id);
      }

      void VertexArray::Bind() const {
        glBindVertexArray(id);
      }

      void VertexArray::Unbind() const {
        glBindVertexArray(0);
      }

      Context::Context(Bored::Window::Window* w)
        : window(w) {
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
      }

      Context::~Context() {}

      void Context::DrawVertexArray(std::shared_ptr<Render::VertexArray> vao, std::shared_ptr<Render::ShaderPipeline> pipeline)
      {
      }

      Bored::Window::Window& Context::GetWindow() const
      {
        return *window;
      }

      bool Context::OnTick(double dt)
      {
          glViewport(0, 0, window->GetWidth(), window->GetHeight());
          glClear(GL_COLOR_BUFFER_BIT);
          glClearColor(0.2f, 0.5f, 0.7f, 1.0f);

          return true;
      }
    }
  }
}
