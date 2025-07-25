#pragma once

#include "../Nodes/Camera.hpp"
#include "Node.hpp"
#include <functional>
#include <iostream>

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
  Scene() {}

  std::shared_ptr<Node> GetRoot() { return root; }

  void SetRoot(std::shared_ptr<Node> new_root) { root = new_root; }

  std::shared_ptr<Camera> GetActiveCamera() { return active_camera; }

  void SetActiveCamera(std::shared_ptr<Camera> new_camera) {
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
  std::shared_ptr<Camera> active_camera;
};
} // namespace Bored
