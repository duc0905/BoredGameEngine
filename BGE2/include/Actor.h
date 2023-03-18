#pragma once
#include "pch.h"
#include "Bases.h"

namespace Bored {
/**
 * @brief Component-based entity
*/
class Actor : public Tickable {
  friend class ActorManager;

private:
  entt::entity id;

public:
  Actor(): id(entt::null) { }
  Actor(const entt::entity& id_) : id(id_) {}
  ~Actor(){};
  operator entt::entity() const { return id; }
  virtual void OnImGui(){};

  inline const bool IsValid() const { return id != entt::null; }
};
}  // namespace Bored
