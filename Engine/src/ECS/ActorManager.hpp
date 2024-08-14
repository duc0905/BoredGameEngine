#pragma once

#include <sstream>
#include <entt/entity/registry.hpp>
#include "Actor.hpp"
#include "./Components/Transform.hpp"
#include "./Components/IDToPtr.hpp"

namespace Bored
{

/**
 * @brief A special data structure to manage actors effectively
 */
class ActorManager
{
  public:
    ActorManager() = default;
    ~ActorManager() = default;
    entt::registry actor_registry;
    /**
     * @brief Create an actor or subtype of actor
     * @tparam T Actor or subtype of Actor
     * @tparam ...Args
     * @tparam
     * @param ...args params to create actor
     * @return shared pointer to the created actor
     */
    template <typename T, class... Args, std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
    std::shared_ptr<T> Create(Args&&... args)
    {
        auto id = actor_registry.create();
        std::shared_ptr<T> a = std::make_shared<T>(args...);
        a->id = id;
        actor_registry.emplace<Bored::ECS::Transform>(id);
        actor_registry.emplace<Bored::ECS::IDToPtr>(id, a);
        return a;
    }

    /**
     * @brief Get components of actors with those components
     * @tparam ...Comps
     * @return entt::view of those components
     */
    template <typename... Comps> decltype(auto) Get()
    {
        return actor_registry.view<Comps...>();
    }

    /**
     * TODO: Fill
     *
     * Description.
     */
    template <typename... Comps> decltype(auto) Get(entt::entity id)
    {
        return actor_registry.try_get<Comps...>(id);
    }

    /**
     * TODO: Fill
     *
     * Description.
     */
    template <typename... Comps> decltype(auto) GetOrCreate(entt::entity id)
    {
        return actor_registry.get_or_emplace<Comps...>(id);
    }

    /**
     * TODO: Fill
     *
     * Description.
     */
    template <typename T, class... Args> T& AddComponent(entt::entity id, Args&&... args)
    {
        if (!actor_registry.valid(id))
        {
            std::stringstream ss;
            ss << "Adding component " << typeid(T).name() << " for invalid actor: " << (int)id;
            // logger->warn(ss.str());
        }

        if (actor_registry.any_of<T>(id))
        {
            std::stringstream ss;
            ss << "Component '" << typeid(T).name() << "' already exist for actor " << (uint64_t)id
               << ". Getting the existed component instead!";
            // logger->warn(ss.str());

            return actor_registry.get<T>(id);
        }

        return actor_registry.emplace_or_replace<T>(id, std::forward<Args>(args)...);
    }

    /**
     * TODO: Fill
     *
     * Description.
     */
    template <typename T> void RemoveComponent(entt::entity id)
    {
        if (typeid(T) == typeid(Bored::ECS::Transform))
        {
            // logger->warn("Cannot remove Transform component.");
            return;
        }

        if (!actor_registry.any_of<T>(id))
        {
            std::stringstream ss;
            ss << "Actor " << (uint64_t)id << " does not have Component '" << typeid(T).name()
               << "'. Not removing any component!";
            // logger->warn(ss.str());
            return;
        }

        actor_registry.remove<T>(id);
        return;
    }

    bool IsValidActor(entt::entity id) const;

    void Delete(entt::entity id);
};

} // namespace Bored
