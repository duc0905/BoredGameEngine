#include "pch.h"
#include "PerspectiveCamera.h"

#include "../Components/PerspectiveCameraComponent.h"

PerspectiveCamera::PerspectiveCamera(const glm::vec4& viewDim,
                                     const glm::vec3& dir,
                                     const glm::vec3& up) {
  camComp = CreateComponent<PerspectiveCameraComponent>(viewDim, dir, up);
  transComp = FindComponent<TransformComponent>();
}

glm::mat4 PerspectiveCamera::GetViewMat() const {
  return camComp->GetViewMat(transComp->GetTranslation(),
                             transComp->GetRotation());
}

glm::mat4 PerspectiveCamera::GetProjectionMat() const {
  return camComp->GetProjectionMat();
}
