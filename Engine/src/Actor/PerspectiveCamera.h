#pragma once
#include "../pch.h"
#include "BaseCamera.h"
#include "../Components/PerspectiveCameraComponent.h"

class PerspectiveCamera : public BaseCamera {
 public:
  PerspectiveCamera(const glm::vec4& viewDim,
                    const glm::vec3& dir = glm::vec3{1.0f, 0.0f, 0.0f},
                    const glm::vec3& up = glm::vec3{0.0f, 0.0f, 1.0f});
  // Inherited via BaseCamera
  virtual glm::mat4 GetViewMat() const override;
  virtual glm::mat4 GetProjectionMat() const override;
  virtual ~PerspectiveCamera() override {}

 private:
  std::shared_ptr<PerspectiveCameraComponent> camComp;
  std::shared_ptr<TransformComponent> transComp;
};
