#pragma once

#include <glm/glm.hpp>
#include "Systems/I_System.hpp"

struct GravityComponent {
  // Mass in kilograms
  float mass;
};

struct MovementComponent {
  // Velocity direction
  glm::vec3 velocity;

  // Acceleration direction
  glm::vec3 acceleration;
};

class GravitySystem : public Bored::I_System {
public:
  virtual void OnUpdate(double dt, Bored::Scene &scene) override;

public:
  static constexpr float G = 6.67e-11;
};
