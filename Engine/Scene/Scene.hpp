#pragma once

#include "../Components/NodeComponent.hpp"
#include "../Systems/I_System.hpp"
// clang-format off
#include "../Systems/Input/IOService.hpp"
#include "Node.hpp"
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <chrono>

namespace Bored {
/**
 * Holds pointers to services used by this scene
 */
struct SceneContext {
  IOService *io;
};

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
  virtual ~Scene() = default;

  /**
   * Function used to create initial nodes in the scene.
   */
  virtual void BuildScene() {}

  /**
   * Called after building the scene.
   *
   * Can put pre-processing optimization code here.
   */
  virtual void AfterBuildScene() {}

  /**
   * Function called on every frames.
   */
  virtual void Update(double dt) {
    for (auto system : systems) {
      system->OnUpdate(dt, *this);
    }
  }

  /**
   * The game loop.
   */
  void GameLoop() {
    bool running = true;
    std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();

    // Main loop
    while (running) {
      std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();

      // Elapsed time since last frame in seconds
      float dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
        .count() /
        1000.0f;

      // Update scene here
      Update(dt);

      running &= !ShouldStop();

      prev = now;
    }
  }

  std::shared_ptr<Node> GetRoot() { return root; }

  void SetRoot(std::shared_ptr<Node> new_root) { root = new_root; }

  std::shared_ptr<Node> CreateNode() {
    std::shared_ptr<Node> node =
        std::shared_ptr<Node>(new Node(*this, ecs_registry));
    node->AddComponent<NodeComponent>(node);
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

  bool ShouldStop() {
    bool ret = false;
    for (auto system : systems) {
      ret |= system->ShouldStop(*this);
    }
    return ret;
  }

public:
  SceneContext context;
  std::vector<std::shared_ptr<I_System>> systems;

protected:
  std::shared_ptr<Node> root;
  std::shared_ptr<Node> active_camera;
};
} // namespace Bored
