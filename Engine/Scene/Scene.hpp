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
  std::shared_ptr<IOService> io;
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
public: entt::registry ecs_registry;

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
  virtual void Update(double dt);

  /**
   * The game loop.
   */
  void GameLoop();

  std::shared_ptr<Node> GetRoot();

  void SetRoot(std::shared_ptr<Node> new_root);

  std::shared_ptr<Node> CreateNode();

  std::shared_ptr<Node> GetActiveCamera();

  void SetActiveCamera(std::shared_ptr<Node> new_camera);

  void TraverseForward(std::function<void(std::shared_ptr<Node>)> visitor);
  void TraverseBackward(std::function<void(std::shared_ptr<Node>)> visitor);

  bool ShouldStop();

public:
  SceneContext context;
  std::vector<std::shared_ptr<I_System>> systems;

protected:
  std::shared_ptr<Node> root;
  std::shared_ptr<Node> active_camera;
};
} // namespace Bored
