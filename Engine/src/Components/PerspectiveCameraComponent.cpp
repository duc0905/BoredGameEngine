
#include "pch.h"
#include "PerspectiveCameraComponent.h"

PerspectiveCameraComponent::PerspectiveCameraComponent(
    const glm::vec4 viewDim,
    const glm::vec3& dir,
    const glm::vec3& up)
    : projMat(glm::perspective(viewDim[0], viewDim[1], viewDim[2], viewDim[3])), up_(up)
{ }

const std::string& PerspectiveCameraComponent::GetComponentName()
{
    // // O: insert return statement here
    return "Perspective Camera";
}

const glm::mat4& PerspectiveCameraComponent::GetViewMat(const glm::vec3& position_)
{
    return glm::lookAt(position_, position_ + dir_, up_);
}

const glm::mat4& PerspectiveCameraComponent::GetProjectionMat()
{
    return projMat;
}
