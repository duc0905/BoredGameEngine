#pragma once
#include "../Components/UpdateComponent.hpp"
#include "../Scene/Scene.hpp"
#include "I_System.hpp"

namespace Bored {
class UpdateSystme : public I_System {
public:
  virtual void OnUpdate(double dt, Scene &scene) override {
    auto view = scene.ecs_registry.view<NodeComponent, UpdateComponent>();

    for (auto &&[entity, node_comp, update_comp] : view.each()) {
      if (update_comp.updater)
        update_comp.updater->OnUpdate(dt, scene, node_comp.node);
    }
  }
};
} // namespace Bored
