#pragma once

#include "../ECS/Node.hpp"

namespace Bored {

class Light : public Node {
public:
  virtual ~Light() = default;

public:
  glm::vec3 light_color = {1.0f, 1.0f, 1.0f};
};

class PointLight : public Light {
public:
  float strength = 1.0f;
};

class DirectionalLight : public Light {
public:
  const glm::vec3 GetDirection() const {
    return glm::normalize(glm::mat3(transform.GetTransformMatrix()) *
                          glm::vec3(0.0f, -1.0f, 0.0f));
  }
};
} // namespace Bored
