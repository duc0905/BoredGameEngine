#pragma once
#include "../../pch.h"
#include "ITexture.h"

class OpenGLTexture : public ITexture {
private:
	GLuint ID;
	std::string filepath_;
	unsigned char* LocalBuffer;
	int Width, Height, BPP;
public:
	OpenGLTexture();
	OpenGLTexture(OpenGLTexture& other) noexcept;
	~OpenGLTexture();

	virtual bool Load(const std::string& filepath) override;
	virtual bool Load(int width, int height, int bbp, void* data) override;

	virtual void Bind(GLuint slot = 0) override;
	virtual void Unbind() override;

	virtual inline int GetWidth() const override { return Width; } 
	virtual inline int GetHeight() const override { return Height; } 
};
