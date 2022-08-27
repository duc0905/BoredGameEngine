#pragma once
#include "../../pch.h"

class ITexture
{
public:
	virtual bool Load(const std::string& filepath) = 0;
	virtual bool Load(int width, int height, int bbp, void* data) = 0;

	virtual void Bind(GLuint slot = 0) = 0;
	virtual void Unbind() = 0;

	virtual inline int GetWidth() const = 0;
	virtual inline int GetHeight() const = 0;
};
