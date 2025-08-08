#include "Gravity.hpp"
#include "Components/TransformComponent.hpp"
#include "Scene/Scene.hpp"
#include <glm/geometric.hpp>

void GravitySystem::OnUpdate(double dt, Bored::Scene &scene) {
  auto gravity_view =
      scene.ecs_registry.view<Bored::TransformComponent, GravityComponent>();

  auto movement_view =
      scene.ecs_registry.view<Bored::TransformComponent, MovementComponent>();

  // Update velocity and location
  for (auto &&[_, trans_comp, move_comp] : movement_view.each()) {
    trans_comp.translate +=
        (move_comp.velocity * (float)dt) +
        (0.5f * move_comp.acceleration * (float)dt * (float)dt);
    move_comp.velocity += (float)dt * move_comp.acceleration;
  }

  // Calculate the force the gravity forces on the moving objects
  for (auto &&[_, grav_trans_comp, gravity_comp] : gravity_view.each()) {
    for (auto &&[_, move_trans_comp, move_comp] : movement_view.each()) {
      glm::vec3 dir = grav_trans_comp.translate - move_trans_comp.translate;
      float d = glm::length(dir);
      glm::vec3 g = GravitySystem::G * gravity_comp.mass * dir / d;
      move_comp.acceleration = g;
    }
  }
}
