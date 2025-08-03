#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Bored {
struct TransformComponent {
  // Transformation
  glm::vec3 translate{0.0f, 0.0f, 0.0f};
  glm::vec3 rotate{0.0f, 0.0f, 0.0f};
  glm::vec3 scale{1.0f, 1.0f, 1.0f};

  glm::mat4 GetTransformMatrix() const {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), translate);

    // Rotation is applied in ZYX order (roll, pitch, yaw)
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f), rotate.z, glm::vec3(0, 0, 1)) *
        glm::rotate(glm::mat4(1.0f), rotate.y, glm::vec3(0, 1, 0)) *
        glm::rotate(glm::mat4(1.0f), rotate.x, glm::vec3(1, 0, 0));

    glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

    return translation * rotation * scaling;
  }
};

} // namespace Bored
