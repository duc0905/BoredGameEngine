#pragma once
#include "../Texture/OpenGLTexture.h"

class RenderBuffer
{
private:
	GLuint ID;
public:
	RenderBuffer(GLuint width, GLuint height);
	~RenderBuffer();
	GLuint GetId() { return ID; }
};

class FrameBuffer
{
private:
	GLuint ID;
public:
	FrameBuffer();
	~FrameBuffer();
	bool Bind();
	bool Unbind();
	bool AttachColorBuffer(std::shared_ptr<OpenGLTexture> colorbuf, int id);
	bool AttachRenderBuffer(std::shared_ptr<RenderBuffer> renderBuf);
	//bool HasColorBuffer();
	//bool HasDepthBuffer();
	//bool HasStencilBuffer();
	bool ClearBuffer();
};

