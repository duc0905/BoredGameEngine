#include "OGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Bored {
    namespace Render {
        namespace OGL {
            Buffer::Buffer() {
                glGenBuffers(1, &id);
            }

            Buffer::~Buffer() {
                glDeleteBuffers(1, &id);
            }

            void Buffer::SubData(std::vector<char> data, BufferLayout bl) {
                glNamedBufferSubData(id, 0, data.size(), &data[0]);
            }
            std::vector<char> Buffer::GetData() {
                std::vector<char> data(size, 0);
                glNamedBufferSubData(id, 0, size, &data[0]);
                return data;
            }

            VertexBuffer::VertexBuffer() : OGL::Buffer() {}
            VertexBuffer::~VertexBuffer() {}

            void VertexBuffer::Bind() {
                glBindBuffer(GL_ARRAY_BUFFER, id);
            }
            void VertexBuffer::Unbind() {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            IndexBuffer::IndexBuffer() : OGL::Buffer() {}
            IndexBuffer::~IndexBuffer() { }

            void IndexBuffer::Bind() {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            }
            void IndexBuffer::Unbind() {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
        }
    }
}
