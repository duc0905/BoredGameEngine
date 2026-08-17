#include "Scene.hpp"

#include <chrono>
#include <stdexcept>

#include "../Components/NodeComponent.hpp"

namespace Bored {
void Scene::Update(double dt) {
  for (auto system : systems) {
    system->OnUpdate(dt, *this);
  }
}

void Scene::GameLoop() {
  bool running = true;
  std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();

  // Main loop
  while (running) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();

    // Elapsed time since last frame in seconds
    float dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
                   .count() /
               1000.0f;

    // Update scene here
    Update(dt);

    running &= !ShouldStop();

    prev = now;
  }
}

std::shared_ptr<Object> Scene::GetRoot() { return root; }

void Scene::SetRoot(std::shared_ptr<Object> new_root) {
  if (root) {
    TraverseBackward([](std::shared_ptr<Object> node) {
      node->is_in_scene = false;
      if (node->HasComponent<BehaviourComponent>()) {
        auto& behaviourComp = node->GetComponent<BehaviourComponent>();

        if (behaviourComp.behaviour) behaviourComp.behaviour->OnDetach();
      }
    });
  }

  root = new_root;
  TraverseForward([](std::shared_ptr<Object> node) {
    node->is_in_scene = true;
    if (node->HasComponent<BehaviourComponent>()) {
      auto& behaviourComp = node->GetComponent<BehaviourComponent>();

      if (behaviourComp.behaviour) behaviourComp.behaviour->OnAttach();
    }
  });
}

std::shared_ptr<Object> Scene::CreateNode() {
  std::shared_ptr<Object> node = Object::Create(*this, ecs_registry);
  return node;
}

std::shared_ptr<Object> Scene::GetActiveCamera() { return active_camera; }

void Scene::SetActiveCamera(std::shared_ptr<Object> new_camera) {
  active_camera = new_camera;
}

void Scene::TraverseForward(
    std::function<void(std::shared_ptr<Object>)> visitor) {
  if (!root) {
    std::cout << "[Warning]: Scene has no root" << std::endl;
    throw std::runtime_error("Scene has no root");
  }

  root->TraverseForward(visitor);
}

void Scene::TraverseBackward(
    std::function<void(std::shared_ptr<Object>)> visitor) {
  if (!root) {
    std::cout << "[Warning]: Scene has no root" << std::endl;
    throw std::runtime_error("Scene has no root");
  }

  root->TraverseBackward(visitor);
}

bool Scene::ShouldStop() {
  bool ret = false;
  for (auto system : systems) {
    ret |= system->ShouldStop(*this);
  }
  return ret;
}

}  // namespace Bored
