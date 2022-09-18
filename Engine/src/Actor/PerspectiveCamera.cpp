#include "pch.h"
#include "PerspectiveCamera.h"
#include "../Components/PerspectiveCameraComponent.h"

PerspectiveCamera::PerspectiveCamera(const glm::vec4& viewDim, const glm::vec3& lookAt, const glm::vec3& up)
{
    camComp = CreateComponent<PerspectiveCameraComponent>(viewDim, lookAt, up);
    transComp = FindComponent<TransformComponent>();
}

glm::mat4 PerspectiveCamera::GetViewMat() const
{
    return camComp->GetViewMat(transComp->GetTranslation());
}

glm::mat4 PerspectiveCamera::GetProjectionMat() const
{
    return camComp->GetProjectionMat();
}
