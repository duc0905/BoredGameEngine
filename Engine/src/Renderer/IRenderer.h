#pragma once
#include "../pch.h"
#include "../System.h"
#include "../World/World.h"
#include "../Actor/BaseCamera.h"

#include "Mesh.h"
#include "ShaderClass.h"

class IRenderer :
    public System
{
    static std::shared_ptr<IRenderer> defaultRenderer_;
public:
    static std::shared_ptr<IRenderer> GetDefault()
    {
        return defaultRenderer_;
    }

    virtual ~IRenderer() {};
    virtual void Render(IWorld& world) = 0;

    virtual void Draw(const Actor& actor) = 0;

    virtual void UseCamera(std::shared_ptr<BaseCamera> cam) = 0;
};

class NullRenderer : public IRenderer
{
	// Inherited via IRenderer
	inline virtual void Render(IWorld& world) override {};
	inline virtual void Draw(const Actor& actor) override {};
	inline virtual void UseCamera(std::shared_ptr<BaseCamera> cam) override {};
};