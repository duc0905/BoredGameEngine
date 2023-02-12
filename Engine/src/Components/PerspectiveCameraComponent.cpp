#include "pch.h"
#include "PerspectiveCameraComponent.h"

PerspectiveCameraComponent::PerspectiveCameraComponent(const glm::vec4 viewDim,
                                                       const glm::vec3& dir,
                                                       const glm::vec3& up)
    : CameraComponent(dir, up), projMat(
          glm::perspective(viewDim[0], viewDim[1], viewDim[2], viewDim[3])){
}

const std::string& PerspectiveCameraComponent::GetComponentName() {
  return "Perspective Camera";
}

const glm::mat4& PerspectiveCameraComponent::GetViewMat(
    const glm::vec3& position_) {
  return glm::lookAt(position_, position_ + dir_, up_);
}

const glm::mat4& PerspectiveCameraComponent::GetViewMat(
    const glm::vec3& position_, const glm::vec3& rotation_) {
  glm::vec3 direction;
  direction.x = cos(yaw) * cos(pitch);
  direction.y = sin(pitch);
  direction.z = sin(yaw);
  dir_ = direction;

  return GetViewMat(position_);
}

const glm::mat4& PerspectiveCameraComponent::GetProjectionMat() {
  return projMat;
}
