#pragma once
#include "../pch.h"
#include "CameraComponent.h"

class PerspectiveCameraComponent : public CameraComponent {
 public:
  PerspectiveCameraComponent(const glm::vec4 viewDim,
                             const glm::vec3& dir = {1.0f, 0.0f, 0.0f},
                             const glm::vec3& up = {0.0f, 0.0f, 1.0f});
  // Inherited via CameraComponent
  virtual const std::string& GetComponentName() override;
  virtual const glm::mat4& GetViewMat(const glm::vec3& position_) override;
  virtual const glm::mat4& GetProjectionMat() override;
  virtual const glm::mat4& GetViewMat(const glm::vec3& position_,
                                      const glm::vec3& rotation_) override;

  inline float AddYaw(float y) { return yaw += y; };
  inline float AddPitch(float p) { return pitch += p; }

  float pitch = 0.0f, yaw = 0.0f;

 private:
  glm::mat4 projMat;
};
