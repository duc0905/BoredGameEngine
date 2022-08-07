#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: RendererID(0), filepath(path), LocalBuffer(nullptr), Width(0), Height(0), BPP(0)
{
	stbi_set_flip_vertically_on_load(0);
	LocalBuffer = stbi_load(path.c_str(), &Width, &Height, &BPP, 4);

	GLCall(glGenTextures(1, &RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (LocalBuffer)
		stbi_image_free(LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &RendererID));
}

void Texture::Bind(GLuint slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, RendererID));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
