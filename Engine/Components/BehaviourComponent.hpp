#pragma once

// #include "../Scene/Node.hpp"
#include <memory>

namespace Bored {
class BehaviourSystem;
class Object;

/**
 * Contains user-defined code for individual Nodes.
 *
 * Support multiple lifecycle methods.
 *
 * @example
 * // Description of my example.
 * @todo Write me later
 */
class Behaviour {
  friend class BehaviourSystem;
  friend class Object;

public:
  virtual ~Behaviour() {}

  /**
   * Lifecycle method.
   *
   * Called when the corresponding node is being attached to he scene. All
   * descendant nodes are also attached to the scene, and their corresponding
   * OnAttach are called by the Scene. Do not call descendants' OnAttach here.
   */
  virtual void OnAttach() {}

  /**
   * Lifecycle method.
   *
   * Called once per render frame. All descendant nodes are also updated, and
   * their corresponding OnUpdate are called by the Scene. Do not call
   * descendants' OnUpdate here.
   */
  virtual void OnUpdate(float dt) {}

  /**
   * Lifecycle method.
   *
   * Called when the corresponding node is being detached from he scene. All
   * descendant nodes are also detached from the scene, and their corresponding
   * OnDetach are called by the Scene. Do not call descendants' OnDetach here.
   */
  virtual void OnDetach() {}

protected:
  // Component access helpers for users
  template <typename T> T &GetComponent();
  template <typename T> bool HasComponent();
  template <typename T, typename... Args> T &AddComponent(Args &&...args);

protected:
  std::shared_ptr<Object> obj; /*< Reference to the object this belongs to. */
protected:
  Behaviour() = default;
};

/**
 * Component holding user-defined code.
 */
struct BehaviourComponent {
  std::shared_ptr<Behaviour> behaviour;
};
} // namespace Bored
