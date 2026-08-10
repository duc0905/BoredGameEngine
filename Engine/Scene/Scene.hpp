#pragma once

#include "../Systems/I_System.hpp"
// clang-format off
#include "../Systems/Input/IOService.hpp"
// clang-format on
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <functional>
#include <memory>

#include "Node.hpp"

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
  virtual void Update(double dt);

  /**
   * The game loop.
   */
  void GameLoop();

  std::shared_ptr<Node> GetRoot();

  /**
   * Set the root of the scene.
   *
   * Only replace the root of the scene with new_root. The old root is not
   * explicitly deleted and is still usable.
   *
   * @param new_root std::shared_ptr<Node> the new root to the scene.
   */
  void SetRoot(std::shared_ptr<Node> new_root);

  /**
   * Initalize a new node associated with this scene.
   *
   * @return std::shared_ptr<Node> the newly created node.
   */
  std::shared_ptr<Node> CreateNode();

  /**
   * Returns the camera being used to render the scene.
   *
   * @return std::shared_ptr<Node> the node with Camera component being used.
   */
  std::shared_ptr<Node> GetActiveCamera();

  /**
   * Set the camera to be used to render the scene.
   *
   * @param std::shared_ptr<Node> the node to be used as the camera. Make sure
   * to have a CameraComponent.
   *
   * @todo Check if new_camera has a CameraComponent
   */
  void SetActiveCamera(std::shared_ptr<Node> new_camera);

  /**
   * Traverse the scene tree.
   *
   * Traverse from the root following DFS path. visitor is called upon the node
   * is visited.
   */
  void TraverseForward(std::function<void(std::shared_ptr<Node>)> visitor);

  /**
   * Traverse the scene tree.
   *
   * Traverse from the root following DFS path. visitor is called after all
   * children are visited.
   */
  void TraverseBackward(std::function<void(std::shared_ptr<Node>)> visitor);

  /**
   * Check if the scene should stop.
   */
  bool ShouldStop();

 public:
  SceneContext context;
  std::vector<std::shared_ptr<I_System>> systems;

 protected:
  std::shared_ptr<Node> root;
  std::shared_ptr<Node> active_camera;
};
}  // namespace Bored
