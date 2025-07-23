#pragma once
#include <functional>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>

namespace Bored {
class InputEvent {};

struct Transform {
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

class Node {
public:
  Node *parent;
  std::vector<std::shared_ptr<Node>> children;

public:
  void AddChild(std::shared_ptr<Node> child) {
    child->parent = this;
    children.push_back(child);
  }

  void Traverse(std::function<void(Node &)> visitor) {
    visitor(*this);
    for (auto &child : children) {
      child->Traverse(visitor);
    }
  }

  Node &GetRootNode() {
    if (parent == nullptr)
      return *this;
    return parent->GetRootNode();
  }

  glm::mat4 GetModelMatrix() const {
    glm::mat4 parentModel(1.0f);

    if (parent) {
      parentModel = parent->GetModelMatrix();
    }

    return parentModel * transform.GetTransformMatrix();
  }

public:
  Transform transform;
  bool visible = true;
};

} // namespace Bored
