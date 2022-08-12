#pragma once
#include "IRenderer.h"

class NullRenderer : public IRenderer
{
	// Inherited via IRenderer
	inline virtual void Render(World& world) override {};
	inline virtual void Draw(const Actor& actor) override {};
	inline virtual void UseCamera(std::shared_ptr<BaseCamera> cam) override {};
};
