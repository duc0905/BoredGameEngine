#include "RenderUtil.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace Bored {
namespace Render {
glm::mat4 OrthoProjector::GetMat() const {
    return glm::ortho(l, r, b, t, 0.1f, 100.0f);
}

glm::mat4 PerspProjector::GetMat() const {
    return glm::perspective(glm::radians(fov), float(w) / float(h), zNear, zFar);
}

glm::vec3 Camera::GetDir() const {
  float z = glm::sin(glm::radians(pitch));
  float x =
      glm::cos(glm::radians(yaw)) * glm::abs(glm::cos(glm::radians(pitch)));
  float y =
      glm::sin(glm::radians(yaw)) * glm::abs(glm::cos(glm::radians(pitch)));
  return {x, y, z};
}

glm::mat4 Camera::GetViewMat(const glm::vec3 &pos) const {
    auto dir = GetDir();
    return glm::lookAt(pos, pos + dir, up);
}
}
}
