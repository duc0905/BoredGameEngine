#include "OrthoCameraComponent.h"

OrthoCameraComponent::OrthoCameraComponent(
    const glm::vec4 viewDim, 
    const glm::vec3& lookAt,
    const glm::vec3& up)
    : center_(lookAt), projMat(glm::ortho(viewDim[0] / 200.0f, viewDim[1] / 200.0f, viewDim[2] / 200.0f, viewDim[3] / 200.0f, 0.0f, 1000.0f)), up_(up)
{
}

const std::string& OrthoCameraComponent::GetComponentName()
{
    // // O: insert return statement here
    return "Orthographic Camera";
}

const glm::mat4& OrthoCameraComponent::GetViewMat(const glm::vec3& position_)
{
    // // O: insert return statement here
    return glm::lookAt(position_, center_, up_);
}

const glm::mat4& OrthoCameraComponent::GetProjectionMat()
{
    // // O: insert return statement here
    return projMat;
}
