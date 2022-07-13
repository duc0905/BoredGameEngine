#include "VertexArray.h"

VertexArray::VertexArray()
{
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
	for (auto comp : layout) {
		glEnableVertexArrayAttrib(ID, layoutCount_);
		glVertexAttribPointer(layoutCount_, comp.Count, GetOpenGLTypeOf(comp.Type), comp.Normalized, stride, (void*)comp.Offset);
		layoutCount_++;
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

