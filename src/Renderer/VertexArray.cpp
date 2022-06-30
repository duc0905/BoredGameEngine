#include "VertexArray.h"

VertexArray::VertexArray()
{
	unsigned int tempId;
	std::cout << "Vert arr constructor" << std::endl;
	glGenVertexArrays(1, &tempId);
	ID = tempId;
	std::cout << &ID << std::endl;
	std::cout << "Vert arr constructor end" << std::endl;
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
	std::cout << "===================" << std::endl;
	std::cout << (int)stride << std::endl;
	for (auto comp : layout) {
		std::cout << comp.Name << ": " << (int)comp.Count << " " << (int)comp.Offset << std::endl;
		glEnableVertexArrayAttrib(ID, index);
		glVertexAttribPointer(index, comp.Count, GetOpenGLTypeOf(comp.Type), comp.Normalized, stride, (void*)comp.Offset);
		index++;
	}
	std::cout << "===================" << std::endl;
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

