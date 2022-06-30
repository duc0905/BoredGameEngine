#ifndef _TEXTURE_CLASS_H_
#define _TEXTURE_CLASS_H_


#include <glad/glad.h>
#include <string>
#include "General.h"

class Texture {
private:
	GLuint RendererID;
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

#endif // !_TEXTURE_CLASS_H_
