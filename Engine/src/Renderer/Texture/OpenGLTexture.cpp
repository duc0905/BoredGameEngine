#include "pch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

OpenGLTexture::OpenGLTexture()
	: ID(0), LocalBuffer(nullptr), Width(0), Height(0), BPP(0)
{
	glGenTextures(1, &ID);
}

OpenGLTexture::OpenGLTexture(OpenGLTexture& other) noexcept
{
	// So that when other's destructor is called, OpenGL does not
	// delete the Texture with the ID.
	ID = other.ID;
	other.ID = 0;

	filepath_ = other.filepath_;
	LocalBuffer = other.LocalBuffer;
	Width = other.Width;
	Height = other.Height;
	BPP = other.BPP;
}

std::shared_ptr<OpenGLTexture> OpenGLTexture::CreateColorBuffer(GLuint width, GLuint height)
{
	std::shared_ptr<OpenGLTexture> tex = std::make_shared<OpenGLTexture>();
	glBindTexture(GL_TEXTURE_2D, tex->GetId());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &ID);
	delete[] LocalBuffer;
}

bool OpenGLTexture::Load(const std::string& filepath)
{
	try
	{
		stbi_set_flip_vertically_on_load(0);
		LocalBuffer = stbi_load(filepath.c_str(), &Width, &Height, &BPP, 4);

		Load(Width, Height, BPP, LocalBuffer);
	}
	catch (std::exception e)
	{
		LOG_COLOR(e.what(), COLOR::YELLOW, COLOR::BLACK);
		return false;
	}

	if (LocalBuffer)
		stbi_image_free(LocalBuffer);

	return true;
}

bool OpenGLTexture::Load(int width, int height, int bbp, void* data)
{
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void OpenGLTexture::Bind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void OpenGLTexture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
