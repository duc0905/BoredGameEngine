#include "BehaviourSystem.hpp"

#include "../Components/BehaviourComponent.hpp"

namespace Bored{
void BehaviourSystem::OnUpdate(double dt, Scene &scene) {
  auto view = scene.ecs_registry.view<NodeComponent, BehaviourComponent>();

  for (auto &&[entity, node_comp, update_comp] : view.each()) {
    if (update_comp.behaviour)
      update_comp.behaviour->OnUpdate(dt);
  }
}
}
