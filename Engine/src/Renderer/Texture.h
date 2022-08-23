#pragma once
#include "../pch.h"

class Texture {
private:
	GLuint ID;
	std::string filepath;
	unsigned char* LocalBuffer;
	int Width, Height, BPP;
public:
	Texture(const std::string& path);
	~Texture();

	void Bind(GLuint slot = 0);
	void Unbind();

	inline int getWidth() const { return Width; }
	inline int getHeight() const { return Height; }
};
