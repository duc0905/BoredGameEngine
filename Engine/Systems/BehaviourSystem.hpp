#pragma once
#include "../Scene/Scene.hpp"
#include "I_System.hpp"

namespace Bored {
class BehaviourSystem : public I_System {
 public:
  /**
   * Inherit from I_System.
   *
   * Get all Behaviour and call OnUpdate. This function should only be called in
   * a Gameloop.
   */
  virtual void OnUpdate(double dt, Scene& scene) override;
};
}  // namespace Bored
