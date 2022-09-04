
#include "../pch.h"
#include "OrthoCameraComponent.h"

OrthoCameraComponent::OrthoCameraComponent(
    const glm::vec4 viewDim, 
    const glm::vec3& lookAt,
    const glm::vec3& up)
    : projMat(glm::ortho(viewDim[0] / 200.0f, viewDim[1] / 200.0f, viewDim[2] / 200.0f, viewDim[3] / 200.0f, 0.0f, 1000.0f)), up_(up)
{ }

const std::string& OrthoCameraComponent::GetComponentName()
{
    // // O: insert return statement here
    return "Orthographic Camera";
}

const glm::mat4& OrthoCameraComponent::GetViewMat(const glm::vec3& position_)
{
    return glm::lookAt(position_, position_ + dir_, up_);
}

const glm::mat4& OrthoCameraComponent::GetProjectionMat()
{
    return projMat;
}
