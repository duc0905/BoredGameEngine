#pragma once

#include <glad/glad.h>
#include <vector>
#include <iostream>
#include "VertexBuffer.h"

class VertexBuffer;

class VertexArray
{
private:
	GLuint ID;
	std::vector<VertexBuffer> _vbs;
	unsigned int layoutCount_ = 0;
public:

	// Constructor that generates a VertexArray ID
	VertexArray();
	~VertexArray();

	void AddVertexBuffer(VertexBuffer& vb);

	// Binds the VertexArray
	void Bind();
	// Unbinds the VertexArray
	void Unbind();
	// Deletes the VertexArray
	void Delete();

	void PrintAttribs(GLenum pname);
};