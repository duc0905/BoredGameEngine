#pragma once
#include "../Components/TransformComponent.hpp"
#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <format>
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
  /**
   * Add a child node.
   */
  void AddChild(std::shared_ptr<Node> child) {
    child->parent = this;
    children.push_back(child);
  }

  /**
   * Helper function letting other code to traverse the scene tree.
   *
   * Traverse from root to every child in DFS order.
   */
  void Traverse(std::function<void(std::shared_ptr<Node>)> visitor) {
    for (auto child : children) {
      if (child) {
        visitor(child);
        child->Traverse(visitor);
      }
    }
  }

  /**
   * Helper function letting other code to traverse from this node back to the
   * root node.
   */
  void Inverse(std::function<void(Node &)> visitor) {
    visitor(*this);
    if (parent != nullptr)
      parent->Inverse(visitor);
  }

  /**
   * Get the root node of the Scene tree.
   */
  [[nodiscard]] Node &GetRootNode() {
    if (parent == nullptr)
      return *this;
    return parent->GetRootNode();
  }

  [[nodiscard]] glm::mat4 GetGlobalTransformMatrix() const {
    if (parent) {
      return parent->GetGlobalTransformMatrix() *
             transform.GetTransformMatrix();
    } else {
      return transform.GetTransformMatrix();
    }
  }

  /**
   * Check if this Node already has Component.
   */
  template <typename Component> bool HasComponent() {
    return registry.any_of<Component>(id);
  }

  /**
   * Create Component and add it to the ECS for this Node.
   */
  template <typename Component, typename... Args>
  Component &AddComponent(Args &&...args) {
    if (HasComponent<Component>()) {
      throw std::runtime_error(std::format("Entity {} already has component {}",
                                           (unsigned int)id,
                                           typeid(Component).name()));
    }

    return registry.emplace<Component>(id, args...);
  }

  /**
   * Get the reference to the Component of this Node.
   */
  template <typename Component> Component &GetComponent() const {
    return registry.get<Component>(id);
  }

private:
  // NOTE: Have to use Scene to create a node.
  Node(Scene &scene, entt::registry &registry)
      : scene(scene), registry(registry), id(registry.create()),
        transform(registry.emplace<Bored::TransformComponent>(id)) {}

public:
  Scene &scene;
  entt::registry &registry;

  Bored::TransformComponent &transform;
  bool visible = true;
};

} // namespace Bored
