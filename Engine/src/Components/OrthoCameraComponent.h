#pragma once
#include "../pch.h"
#include "CameraComponent.h"

class OrthoCameraComponent :
    public CameraComponent
{
public:
    OrthoCameraComponent(const glm::vec4 viewDim);
    OrthoCameraComponent(const glm::vec4 viewDim, const glm::vec3& dir = glm::vec3(100.0f, 0.f, 0.f), const glm::vec3& up = glm::vec3(0.0f, 0.0f, 1.0f));
    // Inherited via CameraComponent
    virtual const std::string& GetComponentName() override;
    virtual const glm::mat4& GetViewMat(const glm::vec3& position_) override;
    virtual const glm::mat4& GetProjectionMat() override;
private:
    glm::mat4 projMat;
};

