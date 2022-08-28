#pragma once
#include "../pch.h"
#include "Component.h"

#include "../Renderer/Texture/ITexture.h"

class TextureComponent : public Component
{
public:
	TextureComponent() = default;
	~TextureComponent() = default;

	// Inherited via Component
	virtual const std::string& GetComponentName() override;

	inline std::shared_ptr<ITexture> GetTexture() const { return texture_; }

private:
	std::shared_ptr<ITexture> texture_;
};

