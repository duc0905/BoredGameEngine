#pragma once

#include "Component.hpp"
#include "TransformComponent.hpp"
#include <glm/glm.hpp>

namespace Bored {

class Light {
public:
  virtual ~Light() = default;

  glm::vec3 light_color = {1.0f, 1.0f, 1.0f};
};

class PointLight : public Light {
  SETUP_COMPONENT(PointLight) {
    RegisterProp<&PointLight::light_color>("Light color");
    RegisterProp<&PointLight::strength>("Strength");
  }

  float strength = 1.0f;
};

class DirectionalLight : public Light {
  SETUP_COMPONENT(DirectionalLight) {
    RegisterProp<&DirectionalLight::light_color>("Light color");
  }

  const glm::vec3 GetDirection(const TransformComponent &transform) const {
    return glm::normalize(glm::mat3(transform.GetTransformMatrix()) *
                          glm::vec3(0.0f, -1.0f, 0.0f));
  }
};
} // namespace Bored
