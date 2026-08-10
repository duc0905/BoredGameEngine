#pragma once
#include "../Scene/Scene.hpp"
#include "I_System.hpp"

namespace Bored {
class BehaviourSystem : public I_System {
public:
  virtual void OnUpdate(double dt, Scene &scene) override;
};
} // namespace Bored
