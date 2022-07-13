#pragma once
#include<glad/glad.h>
#include <string>
#include "VertexArray.h"

class VertexArray;

enum VertLayCompType {
	None = 0,
	Bool,
	Float, Float2, Float3, Float4,
	Int, Int1, Int2, Int3, Int4,
	Mat3, Mat4
};

static unsigned int GetSizeOf(VertLayCompType t) {
	switch (t) {
	case VertLayCompType::Float:  return sizeof(float);
	case VertLayCompType::Float2: return sizeof(float) * 2;
	case VertLayCompType::Float3: return sizeof(float) * 3;
	case VertLayCompType::Float4: return sizeof(float) * 4;
	case VertLayCompType::Int:    return sizeof(int);
	case VertLayCompType::Int2:	  return sizeof(int) * 2;
	case VertLayCompType::Int3:	  return sizeof(int) * 3;
	case VertLayCompType::Int4:	  return sizeof(int) * 4;
	case VertLayCompType::Mat3:	  return sizeof(float) * 9;
	case VertLayCompType::Mat4:	  return sizeof(float) * 16;
	case VertLayCompType::Bool:   return sizeof(bool);
	default: return 0;
	}
}

static uint8_t GetCountOf(VertLayCompType t) {
	switch (t) {
	case VertLayCompType::Float:  return 1;
	case VertLayCompType::Float2: return 2;
	case VertLayCompType::Float3: return 3;
	case VertLayCompType::Float4: return 4;
	case VertLayCompType::Int:    return 1;
	case VertLayCompType::Int2:   return 2;
	case VertLayCompType::Int3:   return 3;
	case VertLayCompType::Int4:   return 4;
	case VertLayCompType::Mat3:   return 9;
	case VertLayCompType::Mat4:   return 16;
	case VertLayCompType::Bool:   return 1;
	default: return 0;
	}
}

static GLenum GetOpenGLTypeOf(VertLayCompType t) {
	switch (t) {
	case VertLayCompType::Float:  return GL_FLOAT;
	case VertLayCompType::Float2: return GL_FLOAT;
	case VertLayCompType::Float3: return GL_FLOAT;
	case VertLayCompType::Float4: return GL_FLOAT;
	case VertLayCompType::Int:    return GL_INT;
	case VertLayCompType::Int2:   return GL_INT;
	case VertLayCompType::Int3:   return GL_INT;
	case VertLayCompType::Int4:   return GL_INT;
	case VertLayCompType::Mat3:   return GL_FLOAT;
	case VertLayCompType::Mat4:   return GL_FLOAT;
	case VertLayCompType::Bool:   return GL_BOOL;
	default: return 0;
	}
}

struct BufferLayoutComp {
	VertLayCompType Type;
	std::string Name;
	unsigned int Size;
	uint8_t Count;
	unsigned int Offset;
	bool Normalized;

	BufferLayoutComp(const std::string& name, VertLayCompType type, bool normalized)
		: Name(name), Type(type), Size(GetSizeOf(type)), Count(GetCountOf(type)), Offset(0), Normalized(normalized) {}


};

class BufferLayout {
	std::vector<BufferLayoutComp> _comps;
	uint32_t _stride = 0;
public:
	BufferLayout() = default;
	BufferLayout(std::initializer_list<BufferLayoutComp> comps);
	uint32_t GetStride();

	std::vector<BufferLayoutComp>::iterator begin() { return _comps.begin(); }
	std::vector<BufferLayoutComp>::iterator end() { return _comps.end(); }
	std::vector<BufferLayoutComp>::const_iterator begin() const { return _comps.begin(); }
	std::vector<BufferLayoutComp>::const_iterator end() const { return _comps.end(); }
};

class VertexBuffer {
private:
	BufferLayout _layout;
	GLuint ID;
	bool IsInit_ = false;
public:
	VertexBuffer() = default;
	VertexBuffer(const void* vertices, GLuint size, GLenum mode = GL_STATIC_DRAW);

	inline BufferLayout GetLayout() const { return _layout; }
	inline void SetLayout(const BufferLayout& layout) { _layout = layout; }

	void SubData(GLsizeiptr size, const void* data);

	void Bind();
	void Unbind();
	void Delete();

	void PrintData();
};
