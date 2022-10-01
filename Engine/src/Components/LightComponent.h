#pragma once
#include "Component.h"
class LightComponent :
    public Component
{
public:
    glm::vec3 color_ = {1.f, 1.f, 1.f};
    float strength_ = 1.f;
};

