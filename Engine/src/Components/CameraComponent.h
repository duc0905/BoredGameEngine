#pragma once
#include "../pch.h"
#include "Component.h"

class CameraComponent :
    public Component
{
protected:
    glm::vec3 dir_ = glm::vec3(1.0f, 0.0f, 0.0f); // direction of the camera
public:
    virtual const glm::mat4& GetViewMat(const glm::vec3& position_) = 0;
    virtual const glm::mat4& GetProjectionMat() = 0;
    void SetDir(const glm::vec3& dir) { dir_ = dir; };
    glm::vec3 GetDir() const { return dir_; };
};

