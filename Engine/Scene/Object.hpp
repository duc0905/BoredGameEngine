#pragma once
#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <format>
#include <functional>
#include <memory>
#include <stdexcept>

#include "../Components/BehaviourComponent.hpp"
#include "../Components/NodeComponent.hpp"
#include "../Components/TransformComponent.hpp"

namespace Bored {

class Scene;

/**
 * A node in the scene hierarchy.
 *
 * A node only store the relationship to other nodes. Avoid storing data in a
 * Node. Use entt (ECS) to store data for the node instead.
 *
 * See Node::AddComponent and Node::HasComponent
 */
struct Object : public std::enable_shared_from_this<Object> {
  friend class Scene;

 public:
  entt::entity id;
  bool is_in_scene = false;

 public:
  /**
   * Add a child node.
   */
  void AddChild(std::shared_ptr<Object> child) {
    if (!child) throw std::runtime_error("child is nullptr");

    if (is_in_scene)
      child->TraverseForward([](std::shared_ptr<Object> node) {
        node->is_in_scene = true;
        if (node->HasComponent<BehaviourComponent>()) {
          auto& behaviourComp = node->GetComponent<BehaviourComponent>();

          if (behaviourComp.behaviour) behaviourComp.behaviour->OnAttach();
        }
      });

    node.children.insert(child);
    child->node.parent = node.self;
  }

  /**
   * Remove a child node and its descendants.
   */
  void RemoveChild(std::shared_ptr<Object> child) {
    if (!child) throw std::runtime_error("child is nullptr");

    auto it = node.children.find(child);

    if (it == node.children.end())
      throw std::runtime_error("child is not a child of this node");
    else {
      if (is_in_scene) {
        // Calling OnDetach on child's descendants.
        child->TraverseBackward([](std::shared_ptr<Object> node) {
          node->is_in_scene = false;
          if (node->HasComponent<BehaviourComponent>()) {
            auto& behaviourComp = node->GetComponent<BehaviourComponent>();
            if (behaviourComp.behaviour) behaviourComp.behaviour->OnDetach();
          }
        });
      }

      child->node.parent = nullptr;
      node.children.erase(it);
    }
  }

  /**
   * Helper function letting other code to traverse the scene tree.
   *
   * Traverse from root to every child in DFS order.
   */
  void TraverseForward(std::function<void(std::shared_ptr<Object>)> visitor) {
    visitor(shared_from_this());
    for (auto child : node.children) {
      if (child) {
        child->TraverseForward(visitor);
      }
    }
  }

  /**
   * Helper function letting other code to traverse the scene tree.
   *
   * Traverse from root to every child in DFS order, visiting the children
   * before visiting parents.
   */
  void TraverseBackward(std::function<void(std::shared_ptr<Object>)> visitor) {
    for (auto child : node.children) {
      if (child) {
        child->TraverseBackward(visitor);
      }
    }
    visitor(shared_from_this());
  }

  /**
   * Helper function letting other code to traverse from this node back to the
   * root node.
   */
  void Inverse(std::function<void(Object&)> visitor) {
    visitor(*this);
    if (node.parent != nullptr) node.parent->Inverse(visitor);
  }

  /**
   * Get the root node of the Scene tree.
   */
  [[nodiscard]] Object& GetRootNode() {
    if (node.parent == nullptr) return *this;
    return node.parent->GetRootNode();
  }

  /**
   * Get global transform mastrix.
   *
   * Parent's transform matrix * local transform matrix.
   */
  [[nodiscard]] glm::mat4 GetGlobalTransformMatrix() const {
    if (node.parent) {
      return node.parent->GetGlobalTransformMatrix() *
             transform.GetTransformMatrix();
    } else {
      return transform.GetTransformMatrix();
    }
  }

  /**
   * Check if this Node already has Component.
   */
  template <typename Component>
  bool HasComponent() {
    return registry.any_of<Component>(id);
  }

  /**
   * Create Component and add it to the ECS for this Node.
   */
  template <typename Component, typename... Args>
  Component& AddComponent(Args&&... args) {
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
  template <typename Component>
  Component& GetComponent() const {
    return registry.get<Component>(id);
  }

 private:
  // NOTE: DO NOT CALL THIS DIRECTLY. USE Create INSTEAD.
  Object(Scene& scene, entt::registry& registry)
      : scene(scene),
        registry(registry),
        id(registry.create()),
        transform(registry.emplace<Bored::TransformComponent>(id)),
        node(registry.emplace<Bored::NodeComponent>(id)) {}

  // NOTE: Have to use Scene to create a node.
  static std::shared_ptr<Object> Create(Scene& scene, entt::registry& registry) {
    auto obj = std::shared_ptr<Object>(new Object(scene, registry));
    obj->node.self = obj->shared_from_this();
    return obj;
  }

 public:
  Scene& scene;
  entt::registry& registry;

  // Component always exist in an object.
  Bored::TransformComponent& transform;
  Bored::NodeComponent& node;

  bool visible = true;
};

}  // namespace Bored

#include "../Components/BehaviourComponentImpl.hpp"
