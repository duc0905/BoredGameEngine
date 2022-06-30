#include "VertexBuffer.h"

/* ========== VertexBuffer ========== */

VertexBuffer::VertexBuffer(const void* vertices, GLuint size, GLenum mode)
	: IsInit_(true)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, mode);
}

void VertexBuffer::SubData(GLsizeiptr size, const void* data) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	Unbind();
}

void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
	std::cout << "DELETETETETET" << std::endl;
	glDeleteBuffers(1, &ID);
}

void VertexBuffer::PrintData()
{
	float vboData[100];
	glGetNamedBufferSubData(ID, 0, sizeof(float) * 28, vboData);

	for (int i = 0; i < 28; i++)
		std::cout << i << ": " << vboData[i] << std::endl;
}

/* ========== BufferLayout ========== */

BufferLayout::BufferLayout(std::initializer_list<BufferLayoutComp> comps) : _comps(comps)
{
	unsigned int offset{ 0 };
	for (auto& comp : _comps) {
		comp.Offset = offset;
		offset += comp.Size;
	}
	uint32_t stride{ 0 };
	for (auto& comp : _comps) {
		stride += comp.Size;
	}
	_stride = stride;
}

uint32_t BufferLayout::GetStride()
{
	return _stride;
}
