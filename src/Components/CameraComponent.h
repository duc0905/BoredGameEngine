#pragma once
#include <glm/glm.hpp>

#include "Component.h"

class CameraComponent :
    public Component
{
    virtual const glm::mat4& GetViewMat(const glm::vec3& position_) = 0;
    virtual const glm::mat4& GetProjectionMat() = 0;
};

