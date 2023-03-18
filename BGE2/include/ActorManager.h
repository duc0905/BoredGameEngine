#pragma once
#include "pch.h"
#include "Bases.h"
#include "Actor.h"

namespace Bored {

struct IDToPtr {
  std::shared_ptr<Actor> ptr;
  IDToPtr(const IDToPtr &o) { ptr = o.ptr; }
  IDToPtr(std::shared_ptr<Actor> p) : ptr(p) {}
  operator std::shared_ptr<Actor>() { return ptr; }
};
/**
 * @brief A special data structure to manage actors effectively
 */
class ActorManager : public Module {
private:
  entt::registry actor_registry;
public:
  template <typename T, class... Args,
            std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
  std::shared_ptr<T> Create(Args &&...args) {
    auto id = actor_registry.create();
    std::shared_ptr<T> a = std::make_shared<T>(args...);
    a->id = id;
    auto s = IDToPtr(a);
    actor_registry.emplace<IDToPtr>(id, s);
    return a;
  }

  template <typename... Comps> entt::view<Comps...> Get() {
    return actor_registry.view<Comps..., IDToPtr>();
  }

  std::shared_ptr<Actor> Get(entt::entity id);
  void Delete(entt::entity id);

  // Inherited via Module
  virtual void OnSetup() override;
  virtual bool OnTick(double dt) override;
  virtual void OnImGui() override;
};
} // namespace Bored
