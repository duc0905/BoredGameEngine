#pragma once

#include "../Scene/Scene.hpp"

namespace Bored {
class Updater {
public:
  virtual void OnUpdate(double dt, Scene& scene, std::shared_ptr<Node> node) = 0;
};

struct UpdateComponent {
  std::shared_ptr<Updater> updater;
};
} // namespace Bored
