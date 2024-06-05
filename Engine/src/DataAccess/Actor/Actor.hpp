#pragma once

#include <entt/entt.hpp>
#include "../../GameLogic.hpp"

namespace Bored {
/**
   * @brief Component-based entity
   */
class Actor : public Bored::Module
{
public:
	entt::entity id;
	Actor() : id(entt::null) {}
	Actor(const entt::entity& id_) : id(id_) {}
	~Actor() {};
	operator entt::entity() const { return id; }
};

}
