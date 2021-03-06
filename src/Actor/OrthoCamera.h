#pragma once
#include "../Components/OrthoCameraComponent.h"

#include "BaseCamera.h"

class OrthoCamera :
    public BaseCamera
{
public:
    OrthoCamera(const glm::vec4& viewDim, const glm::vec3& lookAt = glm::vec3(0.0f), const glm::vec3& = glm::vec3(0.0f, 0.0f, 1.0f));
    // Inherited via BaseCamera
    virtual glm::mat4 GetViewMat() const override;
    virtual glm::mat4 GetProjectionMat() const override;
    virtual ~OrthoCamera() override {}
private:
    std::shared_ptr<OrthoCameraComponent> camComp;
    std::shared_ptr<TransformComponent> transComp;
};

