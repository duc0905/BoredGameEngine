#ifndef IndexBuffer_CLASS_H
#define IndexBuffer_CLASS_H

#include<glad/glad.h>

class IndexBuffer {
private:
	GLuint ID;
	bool IsInit_ = false;
public:
	IndexBuffer() = default;
	IndexBuffer(GLuint* indices, GLuint size, GLenum mode = GL_STATIC_DRAW);
	~IndexBuffer();

	void Bind();
	void Unbind();
	void Delete();
};

#endif // !IndexBuffer_CLASS_H
