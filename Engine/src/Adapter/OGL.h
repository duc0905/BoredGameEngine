#pragma once

#include <vector>
#include <glad/glad.h>
#include "Render.h"
#include "Window.h"

namespace Bored {
  namespace Render {
    namespace OGL {
      class Buffer : public Render::Buffer {
      public:
        Buffer();
        virtual ~Buffer();
      protected:
        GLuint id;
        // Size in bytes
        unsigned int size;
      };

      class VertexBuffer : public Buffer, public Render::VertexBuffer {
      public:
        VertexBuffer();
        virtual ~VertexBuffer();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void SubData(std::vector<char>, BufferLayout) override;
        std::vector<char> GetData() override;
      };

      class IndexBuffer : public Buffer, public Render::IndexBuffer {
      public:
        IndexBuffer();
        virtual ~IndexBuffer();

        virtual void Bind() override;
        virtual void Unbind() override;

        // Inherited via Buffer
        void SubData(std::vector<unsigned int>) override;
        std::vector<char> GetData() override;
      };

      class VertexArray : public Render::VertexArray {
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

      class Context : public Render::Context {
      public:
        Context(Bored::Window::Window* w);
        virtual ~Context();

        virtual bool OnTick(double dt) override;

        virtual void DrawVertexArray(
          std::shared_ptr<Render::VertexArray> vao,
          std::shared_ptr<Render::ShaderPipeline> pipeline) override;

        Bored::Window::Window& GetWindow() const;
      private:
        Bored::Window::Window* window;
      };
    }
  }
}
