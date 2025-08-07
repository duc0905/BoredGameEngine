#pragma once

namespace Bored {
class Scene;

/**
 * The S in ECS
 *
 * Hold the logic and access to the entity, scene tree, and components.
 * Responsible for changing the state of the components.
 */
class I_System {
public:
  virtual ~I_System() = default;

  /**
   * Called once per game iteration.
   */
  virtual void OnUpdate(double dt, Scene &scene) = 0;

  /**
   * Query to see if the game should stop.
   */
  virtual bool ShouldStop(Scene &scene) { return false; }
};
} // namespace Bored
