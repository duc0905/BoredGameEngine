#pragma once
#include "../Components/TransformComponent.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <functional>
#include <memory>
#include <vector>

namespace Bored {

class Scene;

struct Node {
  friend class Scene;

public:
  entt::entity id;
  Node *parent = nullptr;
  std::vector<std::shared_ptr<Node>> children;

public:
  void AddChild(std::shared_ptr<Node> child) {
    child->parent = this;
    children.push_back(child);
  }

  void Traverse(std::function<void(std::shared_ptr<Node>)> visitor) {
    for (auto child : children) {
      if (child) {
        visitor(child);
        child->Traverse(visitor);
      }
    }
  }

  void Inverse(std::function<void(Node &)> visitor) {
    visitor(*this);
    if (parent != nullptr)
      parent->Inverse(visitor);
  }

  Node &GetRootNode() {
    if (parent == nullptr)
      return *this;
    return parent->GetRootNode();
  }

private:
  // NOTE: Have to use Scene to create a node.
  Node(entt::registry &registry)
      : registry(registry), id(registry.create()),
        transform(registry.emplace<Bored::TransformComponent>(id)) {}

public:
  Bored::TransformComponent &transform;
  entt::registry &registry;
  bool visible = true;
};

} // namespace Bored
