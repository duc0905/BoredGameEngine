#include "ActorManager.hpp"

namespace Bored {

void ActorManager::Delete(entt::entity id) { actor_registry.destroy(id); }

bool ActorManager::IsValidActor(entt::entity id) const { return actor_registry.valid(id); }
}
