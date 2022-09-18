#pragma once
#include "../../pch.h"
#include "../Texture/ITexture.h"
class OpenGLMaterial
{
public:
	glm::vec3 AmbientColor = glm::vec3(0.0f);
	glm::vec3 DiffuseColor = glm::vec3(0.0f);
	glm::vec3 SpecularColor = glm::vec3(0.0f);

	ITexture* pDiffuse = NULL;
	ITexture* pSpecularExponent = NULL;
};

