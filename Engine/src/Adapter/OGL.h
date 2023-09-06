#pragma once

#include "Render.h"
#include <vector>
#include <glad/glad.h>

namespace Bored {
    namespace Render {
        namespace OGL {
            class Buffer : public Render::Buffer {
                public:
                    Buffer();
                    virtual ~Buffer();

                    virtual void SubData(std::vector<char> data, BufferLayout bl) override;
                    virtual std::vector<char> GetData() override;
                protected:
                    GLuint id;
                    // Size in bytes
                    int size;
            };

            class VertexBuffer : public Buffer, public Render::VertexBuffer {
                public:
                    VertexBuffer();
                    virtual ~VertexBuffer();

                    virtual void Bind() override;
                    virtual void Unbind() override;

            };

            class IndexBuffer : public Buffer, public Render::IndexBuffer {
                public:
                    IndexBuffer();
                    virtual ~IndexBuffer();

                    virtual void Bind() override;
                    virtual void Unbind() override;
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
                    Context();
                    virtual void DrawVertexArray(
                            std::shared_ptr<Render::VertexArray> vao,
                            std::shared_ptr<Render::ShaderPipeline> pipeline) override;
            };
        }
    }
}
