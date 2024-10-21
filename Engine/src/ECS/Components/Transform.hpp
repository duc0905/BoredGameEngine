#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <nlohmann/json.hpp>

namespace Bored {
namespace ECS {

using json = nlohmann::json;

struct Transform {
  glm::vec3 pos = {0.0f, 0.0f, 0.0f};
  glm::vec3 scale = {1.0f, 1.0f, 1.0f};
  glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

  [[nodiscard]] const glm::mat4 GetMat() const {
    // auto ans = glm::mat4(1.0f);
    auto m = glm::mat4(1.0f);
    // model
    m = glm::translate(glm::mat4(1.0f), pos);
    m = glm::rotate(m, glm::radians(glm::length(rotation)),
                    glm::normalize(rotation));
    m = glm::scale(m, scale);
    return m;
  }
};

void to_json(json& j, Transform& transform);

void from_json(const json& j, Transform& transform);

}  // namespace ECS
}  // namespace Bored
