#include "VertexArray.h"

VertexArray::VertexArray()
{
	std::cout << "Vert arr constructor" << std::endl;
	glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray()
{
}

void VertexArray::AddVertexBuffer(VertexBuffer& vb)
{
	_vbs.push_back(vb);

	Bind();
	vb.Bind();
	auto layout = vb.GetLayout();

	uint16_t stride = layout.GetStride();
	int index = 0;
	for (auto comp : layout) {
		glEnableVertexArrayAttrib(ID, index);
		glVertexAttribPointer(index, comp.Count, GetOpenGLTypeOf(comp.Type), comp.Normalized, stride, (void*)comp.Offset);
		index++;
	}
	vb.Unbind();
}

void VertexArray::Bind()
{
	glBindVertexArray(ID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &ID);
}

void VertexArray::PrintAttribs(GLenum pname)
{
	GLuint data[100];
	glGetVertexAttribIuiv(ID, pname, data);

	std::cout << "VA attrib: " << data[0] << std::endl;
}

