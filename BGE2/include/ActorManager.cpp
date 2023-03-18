#include "pch.h"
#include "ActorManager.h"

namespace Bored {
std::shared_ptr<Actor> ActorManager::Get(entt::entity id) {
  return actor_registry.get<IDToPtr>(id);
}

void ActorManager::Delete(entt::entity id) { actor_registry.destroy(id); }
bool ActorManager::OnTick(double dt) {
  bool shouldClose = false;
  auto view = actor_registry.view<IDToPtr>();
  for (auto id : view) {
    std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
    if (a->OnTick(dt))
      return true;
  }
  return false;
}
void ActorManager::OnImGui() {
  auto view = actor_registry.view<IDToPtr>();
  for (auto id : view) {
    std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
    a->OnImGui();
  }
}
void ActorManager::OnSetup() {}
} // namespace Bored
