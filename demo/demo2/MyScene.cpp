#include "MyScene.hpp"
#include "Components/Camera.hpp"
#include "Components/Lighting.hpp"
#include "Components/Mesh3D.hpp"
#include "Components/MeshComponent.hpp"
#include "Gravity.hpp"
#include "Scene/Node.hpp"
#include "Systems/I_System.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"

MyScene::MyScene() {
  // Setup window system
  window = std::make_shared<Bored::WindowService>(800, 600);
  renderer = std::make_shared<OGL::Renderer>(*window);
  window->AddFrameBufferSizeListener(renderer.get());
  systems.push_back(renderer);

  // Setup input system
  std::shared_ptr<Bored::Input> input =
      std::make_shared<Bored::Input>(*window->input_service);
  systems.push_back(input);

  // Populate the services into scene context
  context.window_service = window.get();
  context.input_service = window->input_service.get();

  // My cucstom system
  std::shared_ptr<Bored::I_System> gravity_system =
      std::make_shared<GravitySystem>();
  systems.push_back(gravity_system);
}

void MyScene::BuildScene() {
  std::shared_ptr<Bored::ArrayMesh> sphere =
      OGL::LoadModel("res/models/sphere.gltf");

  root = CreateNode();

  // Camera
  std::shared_ptr<Bored::Node> camera_node = CreateNode();
  camera_node->AddComponent<Bored::CameraComponent>(new Bored::Perspective(
      Bored::Perspective::GetFOV(1.0f, 2.0f), 800, 600, 1.0f));
  Bored::TransformComponent &camera_transform =
      camera_node->GetComponent<Bored::TransformComponent>();

  camera_transform.translate = {0.0f, 1.0f, 8.0f};
  camera_transform.rotate.x = -glm::pi<float>() / 18.0f;

  root->AddChild(camera_node);
  SetActiveCamera(camera_node);

  // 1 center body with mass
  std::shared_ptr<Bored::Node> central_gravity = CreateNode();
  central_gravity->AddComponent<Bored::MeshComponent>(sphere);
  central_gravity->AddComponent<GravityComponent>(10e10);
  root->AddChild(central_gravity);

  // 1 satelite orbitting
  std::shared_ptr<Bored::Node> orbitor = CreateNode();
  orbitor->AddComponent<Bored::MeshComponent>(sphere);
  auto& movement = orbitor->AddComponent<MovementComponent>();
  movement.velocity = {4.0f, 0.0f, 0.0f};
  orbitor->transform.translate = {0.0f, 5.0f, 0.0f};
  orbitor->transform.scale = {0.3f, 0.3f, 0.3f};
  root->AddChild(orbitor);

  // Setting up light sources
  std::shared_ptr<Bored::Node> dir_light_node = CreateNode();
  Bored::DirectionalLight &dir_light_comp =
      dir_light_node->AddComponent<Bored::DirectionalLight>();
  dir_light_comp.light_color = {0.8f, 0.6f, 0.2f};
  dir_light_node->transform.rotate = {0.3f, 0.1f, 0.0f};
  root->AddChild(dir_light_node);
}

void MyScene::AfterBuildScene() { renderer->SetupObjects(*this); }
