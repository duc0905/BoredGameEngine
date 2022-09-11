#include "pch.h"
#include "FrameBuffer.h"
#include <GLFW/glfw3.h>

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &ID);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &ID);
}

bool FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	return true;
}

bool FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

bool FrameBuffer::AttachColorBuffer(std::shared_ptr<OpenGLTexture> colorbuf, int id)
{
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, colorbuf->GetId(), 0);
	Unbind();
	return true;
}

bool FrameBuffer::AttachRenderBuffer(std::shared_ptr<RenderBuffer> rbo)
{
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo->GetId());
	Unbind();
	return true;
}

bool FrameBuffer::ClearBuffer()
{
	Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Unbind();
	return true;
}

RenderBuffer::RenderBuffer(GLuint width, GLuint height)
{
	glGenRenderbuffers(1, &ID);
	glBindRenderbuffer(GL_RENDERBUFFER, ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

RenderBuffer::~RenderBuffer()
{
	glDeleteRenderbuffers(1, &ID);
}