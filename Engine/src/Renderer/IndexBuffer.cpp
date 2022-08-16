#include "../pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(GLuint* indices, GLuint size, GLenum mode)
	: IsInit_(true)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, mode);
}

IndexBuffer::~IndexBuffer()
{
	//Delete();
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Delete()
{
	glDeleteBuffers(1, &ID);
}
