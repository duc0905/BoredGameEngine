#pragma once

#include "Scene/Scene.hpp"
#include "Systems/I_System.hpp"
#include <glm/glm.hpp>

struct PhysicsComponent {
  glm::vec3 acceleration; /*< The current acceleration upon the entity. */
  glm::vec3 velocity;     /*< The current velocity of the entity. */
  bool grounded = false;
};

/**
 * A simple physics system.
 *
 * Simple gravity system and smooth motion for player movement.
 */
class PhysicsSystem : public Bored::I_System {
  /**
   * Called once per game iteration.
   */
  virtual void OnUpdate(double dt, Bored::Scene &scene) {
    // Gravity
    auto physics_view =
        scene.ecs_registry.view<Bored::TransformComponent, PhysicsComponent>();

    float half_dt_sq = 0.5f * (float)dt * (float)dt;
    for (auto &&[_, trans_comp, phys_comp] : physics_view.each()) {
      trans_comp.translate +=
          (float)dt * phys_comp.velocity + half_dt_sq * phys_comp.acceleration;

      phys_comp.velocity += (float)dt * phys_comp.acceleration;

      // "Gravity"
      // Keep things from falling through the world
      if (trans_comp.translate.y < 1.0f) {
        trans_comp.translate.y = 1.0f;
        phys_comp.grounded = true;
        std::cout << "ground" << std::endl;
      } else {
        phys_comp.velocity.y -= GRAVITY * dt;
        phys_comp.grounded = false;
        std::cout << "fly" << std::endl;
      }
    }
  }

public:
  static constexpr float GRAVITY = 9.8f;
};
