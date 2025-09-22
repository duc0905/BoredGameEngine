#include "MazeScene.hpp"
#include "Components/Camera.hpp"
#include "Components/Lighting.hpp"
#include "Components/Mesh3D.hpp"
#include "Components/MeshComponent.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"
#include <GLFW/glfw3.h>

void PlayerController::OnInput(double dt, Bored::InputEvent &event,
                               std::shared_ptr<Bored::Node> node) {
  if (event.type == Bored::InputType::KEY_DOWN ||
      event.type == Bored::InputType::KEY_REPEAT) {
    // auto movement_comp = node->GetComponent<MovementComponent>();
    glm::vec3 direction(0.0f);

    if (event.key.keyCode == GLFW_KEY_W) {
      direction = {0.0f, 0.0f, -1.0f};
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_A) {
      direction = {-1.0f, 0.0f, 0.0f};
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_S) {
      direction = {0.0f, 0.0f, 1.0f};
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_D) {
      direction = {1.0f, 0.0f, 0.0f};
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_SPACE) {
      direction = {0.0f, 1.0f, 0.0f};
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_LEFT_CONTROL) {
      direction = {0.0f, -1.0f, 0.0f};
      event.handled = true;
    }

    if (event.handled) {
      direction =
          node->transform.GetTransformMatrix() * glm::vec4(direction, 0.0f);

      direction = glm::normalize(direction);

      node->transform.translate += direction * speed * (float)dt;
    }
  }

  if (is_mouse_hidden) {
    if (event.type == Bored::InputType::MOUSE_MOVE) {
      glm::vec3 rotation{-event.mouseMove.dy, -event.mouseMove.dx, 0.0f};
      rotation *= dt * sensivity;
      node->transform.rotate += rotation;
      event.handled = true;
    }
  }

  if (event.type == Bored::InputType::KEY_DOWN &&
      event.key.keyCode == GLFW_KEY_ESCAPE) {
    if (is_mouse_hidden) {
      node->scene.context.io->SetCursorMode(Bored::CursorMode::VISIBLE);
      is_mouse_hidden = false;
    } else {
      node->scene.context.io->SetCursorMode(Bored::CursorMode::DISABLED);
      is_mouse_hidden = true;
    }
  }
}

MazeScene::MazeScene() : asset_manager(Bored::AssetManager::GetInstance()) {
  // Setup window system
  io = std::make_shared<Bored::IOService>(SCR_WIDTH, SCR_HEIGHT);
  renderer = std::make_shared<OGL::Renderer>(*io);
  systems.push_back(renderer);

  // Setup input system
  std::shared_ptr<Bored::Input> input = std::make_shared<Bored::Input>(*io);
  systems.push_back(input);

  // Populate the services into scene context
  context.io = io.get();
}

void MazeScene::BuildScene() {
  // Building scene
  std::shared_ptr<Bored::Node> root = CreateNode();
  SetRoot(root);

  // Player node
  std::shared_ptr<Bored::Node> player_node = CreateNode();
  player_node->AddComponent<Bored::CameraComponent>(new Bored::Perspective(
      Bored::Perspective::GetFOV(1.0f, 2.0f), SCR_WIDTH, SCR_HEIGHT, 1.0f));
  // Player control
  Bored::InputComponent &player_input =
      player_node->AddComponent<Bored::InputComponent>();
  player_input.input_handler = std::make_shared<PlayerController>();
  player_node->transform.translate = {3.0f, 2.0f, 4.0f};
  SetActiveCamera(player_node);
  root->AddChild(player_node);

  // Walls and floors
  // Loading models
  // TODO: should let the asset registry handle this
  std::shared_ptr<Bored::MeshComponent> floor_tile_model =
      asset_manager.LoadModel(resource_path +
                              "models/kaykit_prototype/Primitive_Floor.gltf");
  std::shared_ptr<Bored::MeshComponent> wall_model = asset_manager.LoadModel(
      resource_path + "models/kaykit_prototype/Cube_Prototype_Large_B.gltf");
  // std::shared_ptr<Bored::Material> black_tile_mat =
  //     std::make_shared<Bored::Material>(glm::vec3{0.1f, 0.1f, 0.1f}, 0.1f,
  //     0.7f,
  //                                       0.2f, 1.0f);
  // std::shared_ptr<Bored::Material> white_tile_mat =
  //     std::make_shared<Bored::Material>(glm::vec3{0.9f, 0.9f, 0.9f}, 0.1f,
  //     0.7f,
  //                                       0.2f, 1.0f);
  // std::shared_ptr<Bored::Material> wall_mat =
  // std::make_shared<Bored::Material>(
  //     glm::vec3{0.9f, 0.9f, 0.9f}, 0.1f, 0.7f, 0.2f, 1.0f);

  // Massive 10x10 floor
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      std::shared_ptr<Bored::Node> floor_tile = CreateNode();
      auto &mesh_comp = floor_tile->AddComponent<Bored::MeshComponent>();
      mesh_comp.mesh = floor_tile_model->mesh;
      mesh_comp.material = floor_tile_model->material;
      // if ((i + j) % 2 == 0)
      //   mesh_comp.material = floor_tile_model->material;
      // else
      //   mesh_comp.material = floor_tile_model->material;
      floor_tile->transform.translate = {i * 1.0f, -0.5f, j * 1.0f};
      floor_tile->transform.scale = {0.25, 0.5f, 0.25f};
      root->AddChild(floor_tile);
    }
  }

  for (int i = 0; i < 10; i += 2) {
    for (int j = 0; j < 10; j += 2) {
      std::shared_ptr<Bored::Node> wall = CreateNode();
      auto &mesh_comp = wall->AddComponent<Bored::MeshComponent>();
      mesh_comp.mesh = wall_model->mesh;
      mesh_comp.material = wall_model->material;

      wall->transform.translate = {i * 1.0f, 0.0f, j * 1.0f};
      wall->transform.scale = {0.25f, 0.25f, 0.25f};
      root->AddChild(wall);
    }
  }

  // Lighting
  std::shared_ptr<Bored::Node> dir_light_node = CreateNode();
  Bored::DirectionalLight &dir_light_comp =
      dir_light_node->AddComponent<Bored::DirectionalLight>();
  dir_light_comp.light_color = {1.0f, 1.0f, 1.0f};
  dir_light_node->transform.rotate = {0.3f, 0.1f, 0.0f};
  root->AddChild(dir_light_node);
}

void MazeScene::AfterBuildScene() { renderer->SetupObjects(*this); }
