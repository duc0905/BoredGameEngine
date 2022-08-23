#pragma once
#include "Renderer\Renderer.h"

class MyRenderer : public Renderer
{
    virtual void Render(IWorld& world) override;
};

