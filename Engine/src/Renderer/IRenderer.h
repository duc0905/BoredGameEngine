#pragma once
#include <memory>
#include "../System.h"
#include "../World/World.h"
#include "../Actor/BaseCamera.h"

#include "Mesh.h"
#include "ShaderClass.h"

#include "NullRenderer.h"

class IRenderer :
    public System
{
    static std::shared_ptr<IRenderer> defaultRenderer_;
public:
    static IRenderer& GetDefault()
    {
        return *defaultRenderer_;
    }

    virtual ~IRenderer() {};
    virtual void Render(World& world) = 0;

    virtual void Draw(const Actor& actor) = 0;

    virtual void UseCamera(std::shared_ptr<BaseCamera> cam) = 0;
};

std::shared_ptr<IRenderer> IRenderer::defaultRenderer_ = std::make_shared<NullRenderer>();
