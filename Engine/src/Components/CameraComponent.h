#pragma once
#include "../pch.h"
#include "Component.h"

class CameraComponent : public Component {
 protected:
  glm::vec3 dir_, up_;

 public:
  CameraComponent() = default;
  CameraComponent(glm::vec3 dir, glm::vec3 up) : up_(up), dir_(dir){};
  virtual const glm::mat4& GetViewMat(const glm::vec3& position_) = 0;
  virtual const glm::mat4& GetViewMat(const glm::vec3& position_,
                                      const glm::vec3& rotation_) = 0;
  virtual const glm::mat4& GetProjectionMat() = 0;

  inline glm::vec3 GetUp() const { return up_; }
  inline glm::vec3 GetDir() const { return dir_; }
};
