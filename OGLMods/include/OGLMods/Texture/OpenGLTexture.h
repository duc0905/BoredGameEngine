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

	// Helper function to create Colorbuffer for Framebuffer
	static std::shared_ptr<OpenGLTexture> CreateColorBuffer(GLuint width, GLuint height, GLenum internalFormat, GLenum format);

	inline GLuint GetId() { return ID; };

	virtual bool Load(const std::string& filepath) override;
	virtual bool Load(int width, int height, int bbp, void* data) override;

	virtual void Bind(GLuint slot = 0) override;
	virtual void Unbind() override;

	virtual inline int GetWidth() const override { return Width; } 
	virtual inline int GetHeight() const override { return Height; } 
};
