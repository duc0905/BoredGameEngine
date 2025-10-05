#include "Scene2D.hpp"
#include "Components/FlatComponent.hpp"
#include "Scene/Node.hpp"
#include "Systems/Input/IOService.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Renderer2D/CPU_Renderer2D.hpp"

Scene2D::Scene2D() {
  context.io = std::make_shared<Bored::IOService>(800, 600);

  std::shared_ptr<Bored::CPU_Renderer2D> renderer =
      std::make_shared<Bored::CPU_Renderer2D>(800, 600);
  systems.push_back(renderer);

  // Setup input system
  std::shared_ptr<Bored::Input> input = std::make_shared<Bored::Input>(*context.io);
  systems.push_back(input);
}

void Scene2D::BuildScene() {
  root = CreateNode();

  std::shared_ptr<Bored::Container> el = std::make_shared<Bored::Container>();
  el->pos = {100, 100};
  el->size = {600, 300};
  el->bg_color = {1, 1, 0, 1};

  root->AddComponent<Bored::FlatComponent>(el);
}

void Scene2D::AfterBuildScene() {}
