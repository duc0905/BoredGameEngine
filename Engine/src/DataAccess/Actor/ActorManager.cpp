#define ACTORMANAGER_EXPORTS

#include "ActorManager.hpp"

namespace Bored {

void ActorManager::Delete(entt::entity id) { actor_registry.destroy(id); }

bool ActorManager::IsValidActor(entt::entity id) const { return actor_registry.valid(id); }

void ActorManager::OnSetup() {};

bool ActorManager::OnUpdate(double dt) {
	bool keepRunning = true;
	auto view = actor_registry.view<IDToPtr>();

	for (auto id : view) {
		std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
		keepRunning &= a->OnUpdate(dt);
	}

	return keepRunning;
}

void ActorManager::OnShutdown() {}

}

Bored::ActorManager* CreateInstance() {
	return new Bored::ActorManager();
}

void DeleteInstance(Bored::ActorManager* instance) {
	delete instance;
}
