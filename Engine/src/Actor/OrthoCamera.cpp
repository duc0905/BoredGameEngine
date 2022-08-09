#include "OrthoCamera.h"

OrthoCamera::OrthoCamera(const glm::vec4& viewDim, const glm::vec3& lookAt, const glm::vec3& up)
{
    camComp = CreateComponent<OrthoCameraComponent>(viewDim, lookAt, up);
    transComp = FindComponent<TransformComponent>();
}

glm::mat4 OrthoCamera::GetViewMat() const
{
    return camComp->GetViewMat(transComp->GetTranslation());
}

glm::mat4 OrthoCamera::GetProjectionMat() const
{
    return camComp->GetProjectionMat();
}
