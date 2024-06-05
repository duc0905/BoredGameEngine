#include "ActorManager.hpp"

namespace Bored {

void ActorManager::Delete(entt::entity id) { actor_registry.destroy(id); }

bool ActorManager::OnUpdate(double dt) {
	bool keppRunning = true;
	auto view = actor_registry.view<IDToPtr>();

	for (auto id : view) {
		std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
		keppRunning &= a->OnUpdate(dt);
	}

	return keppRunning;
}
}
