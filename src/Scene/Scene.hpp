#pragma once

#include "../Components/NodeComponent.hpp"
#include "Node.hpp"
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <functional>
#include <iostream>
#include <memory>

namespace Bored {
/**
 * Represents a world/level in the game
 *
 * Containing information specifically to a world/level such as nodes, input
 * mapping, etc.
 *
 * @example
 * // Description of my example.
 * Write me later
 */
class Scene {
public:
  entt::registry ecs_registry;

public:
  Scene() {}

  std::shared_ptr<Node> GetRoot() { return root; }

  void SetRoot(std::shared_ptr<Node> new_root) { root = new_root; }

  std::shared_ptr<Node> CreateNode() {
    std::shared_ptr<Node> node = std::shared_ptr<Node>(new Node(ecs_registry));
    ecs_registry.emplace<NodeComponent>(node->id, node);
    return node;
  }

  std::shared_ptr<Node> GetActiveCamera() { return active_camera; }

  void SetActiveCamera(std::shared_ptr<Node> new_camera) {
    active_camera = new_camera;
  }

  void Traverse(std::function<void(std::shared_ptr<Node>)> visitor) {
    if (!root) {
      std::cout << "[Warning]: Scene has no root" << std::endl;
      return;
    }

    visitor(root);
    root->Traverse(visitor);
  }

private:
  std::shared_ptr<Node> root;
  std::shared_ptr<Node> active_camera;
};
} // namespace Bored
