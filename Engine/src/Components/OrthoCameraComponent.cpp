
#include "../pch.h"
#include "OrthoCameraComponent.h"

OrthoCameraComponent::OrthoCameraComponent(const glm::vec4 viewDim): CameraComponent(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.0f, 0.f, 1.f)),
projMat(glm::ortho(viewDim[0] / 200.0f, viewDim[1] / 200.0f, viewDim[2] / 200.0f, viewDim[3] / 200.0f, 0.0f, 1000.0f))
{}

OrthoCameraComponent::OrthoCameraComponent(
    const glm::vec4 viewDim, 
    const glm::vec3& dir,
    const glm::vec3& up)
    : CameraComponent(dir, up), projMat(glm::ortho(viewDim[0] / 200.0f, viewDim[1] / 200.0f, viewDim[2] / 200.0f, viewDim[3] / 200.0f, 0.0f, 1000.0f))
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

const glm::mat4& OrthoCameraComponent::GetViewMat(const glm::vec3& position_,
                                                  const glm::vec3& rotation_) {
    dir_ = glm::vec3({1.0f, 0.0f, 0.0f}) * rotation_;
    return GetViewMat(position_);
}

const glm::mat4& OrthoCameraComponent::GetProjectionMat()
{
    return projMat; }

