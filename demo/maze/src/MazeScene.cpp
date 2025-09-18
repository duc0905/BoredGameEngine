#include "MazeScene.hpp"
#include "Components/Camera.hpp"
#include "Components/Lighting.hpp"
#include "Components/Mesh3D.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "GravitySystem.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/geometric.hpp>
#include <string>

void PlayerController::UpdateMovement(Bored::TransformComponent &trans,
                                      PhysicsComponent &phys) {
  glm::vec3 direction(0.0f);

  if (keydown[0])
    direction -= glm::vec3{0.0f, 0.0f, 1.0f};
  if (keydown[1])
    direction += glm::vec3{0.0f, 0.0f, 1.0f};
  if (keydown[2])
    direction += glm::vec3{1.0f, 0.0f, 0.0f};
  if (keydown[3])
    direction -= glm::vec3{1.0f, 0.0f, 0.0f};
  // if (keydown[4])
  //   direction += glm::vec3{0.0f, 1.0f, 0.0f};
  // if (keydown[5])
  //   direction -= glm::vec3{0.0f, 1.0f, 0.0f};

  auto mat = trans.GetTransformMatrix();

  direction = mat * glm::vec4(direction, 0.0f);
  direction.y = 0.0f;

  if (glm::length(direction) >= 0.01f) {
    direction = glm::normalize(direction) * speed;
  }

  phys.velocity.x = direction.x;
  phys.velocity.z = direction.z;

  if (keydown[4] && phys.grounded) {
    phys.velocity.y = jump_strength;
    std::cout << "Jump" << std::endl;
  }
}

void PlayerController::OnInput(double dt, Bored::InputEvent &event,
                               std::shared_ptr<Bored::Node> node) {
  if (event.type == Bored::InputType::KEY_DOWN) {
    if (event.key.keyCode == GLFW_KEY_W) {
      keydown[0] = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_A) {
      keydown[3] = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_S) {
      keydown[1] = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_D) {
      keydown[2] = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_SPACE) {
      keydown[4] = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_LEFT_CONTROL) {
      keydown[5] = true;
      event.handled = true;
    }
  }

  if (event.type == Bored::InputType::KEY_UP) {
    if (event.key.keyCode == GLFW_KEY_W) {
      keydown[0] = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_A) {
      keydown[3] = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_S) {
      keydown[1] = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_D) {
      keydown[2] = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_SPACE) {
      keydown[4] = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_LEFT_CONTROL) {
      keydown[5] = false;
      event.handled = true;
    }
  }

  if (event.handled) {
    auto &phys = node->GetComponent<PhysicsComponent>();

    UpdateMovement(node->transform, phys);
  }

  if (is_mouse_hidden) {
    if (event.type == Bored::InputType::MOUSE_MOVE) {
      glm::vec3 rotation{-event.mouseMove.dy, -event.mouseMove.dx, 0.0f};
      rotation *= dt * sensivity;
      node->transform.rotate += rotation;

      auto &phys = node->GetComponent<PhysicsComponent>();

      UpdateMovement(node->transform, phys);
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

MazeScene::MazeScene(const std::string &filepath) {
  // Setup window system
  io = std::make_shared<Bored::IOService>(SCR_WIDTH, SCR_HEIGHT);
  renderer = std::make_shared<OGL::Renderer>(*io);
  systems.push_back(renderer);

  // Setup input system
  std::shared_ptr<Bored::Input> input = std::make_shared<Bored::Input>(*io);
  systems.push_back(input);

  std::shared_ptr<PhysicsSystem> physics = std::make_shared<PhysicsSystem>();
  systems.push_back(physics);

  // Populate the services into scene context
  context.io = io.get();

  // Read from file and populate the data structure
  std::string line;
  std::ifstream level_file(filepath);

  if (level_file.is_open()) {
    std::cout << "[info]: Reading " << filepath
              << " as level file:" << std::endl;
    while (std::getline(level_file, line)) {
      std::cout << line << std::endl;
      std::vector<char> level_row;
      for (char c : line) {
        level_row.push_back(c);
      }
      level.push_back(level_row);
    }

    height = level.size();
    width = level[0].size();
  } else {
    std::cout << "[Error]: Cannot read file " << filepath << std::endl;
  }
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
  PhysicsComponent &phys_comp = player_node->AddComponent<PhysicsComponent>();
  Bored::InputComponent &player_input =
      player_node->AddComponent<Bored::InputComponent>();
  player_input.input_handler = std::make_shared<PlayerController>();
  player_node->transform.translate = {3.0f, 2.0f, 4.0f};
  SetActiveCamera(player_node);
  root->AddChild(player_node);

  // Walls and floors
  // Loading models
  // TODO: should let the asset registry handle this
  std::shared_ptr<Bored::ArrayMesh> floor_tile_model = OGL::LoadModel(
      resource_path + "models/kaykit_prototype/Primitive_Floor.gltf");
  std::shared_ptr<Bored::ArrayMesh> wall_model = OGL::LoadModel(
      resource_path + "models/kaykit_prototype/Primitive_Wall.gltf");
  std::shared_ptr<Bored::Material> black_tile_mat =
      std::make_shared<Bored::Material>(glm::vec3{0.1f, 0.1f, 0.1f}, 0.1f, 0.7f,
                                        0.2f, 1.0f);
  std::shared_ptr<Bored::Material> white_tile_mat =
      std::make_shared<Bored::Material>(glm::vec3{0.9f, 0.9f, 0.9f}, 0.1f, 0.7f,
                                        0.2f, 1.0f);

  // Massive 10x10 floor
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      std::shared_ptr<Bored::Node> floor_tile = CreateNode();
      auto &mesh_comp = floor_tile->AddComponent<Bored::MeshComponent>();
      mesh_comp.mesh = floor_tile_model;
      if ((i + j) % 2 == 0)
        mesh_comp.material = black_tile_mat;
      else
        mesh_comp.material = white_tile_mat;
      floor_tile->transform.translate = {i * 1.0f, -1.0f, j * 1.0f};
      floor_tile->transform.scale = {0.25, 0.5f, 0.25f};
      root->AddChild(floor_tile);
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (level[i][j] == '1') {
        std::shared_ptr<Bored::Node> wall = CreateNode();
        auto &mesh_comp = wall->AddComponent<Bored::MeshComponent>();
        mesh_comp.mesh = wall_model;

        wall->transform.translate = {i * 1.0f, 0.0f, j * 1.0f};
        wall->transform.scale = {0.25f, 0.25f, 0.25f};
        root->AddChild(wall);
      }
    }
  }

  // Lighting
  std::shared_ptr<Bored::Node> dir_light_node = CreateNode();
  Bored::DirectionalLight &dir_light_comp =
      dir_light_node->AddComponent<Bored::DirectionalLight>();
  dir_light_comp.light_color = {0.8f, 0.6f, 0.4f};
  dir_light_node->transform.rotate = {0.3f, 0.1f, 0.0f};
  root->AddChild(dir_light_node);
}

void MazeScene::AfterBuildScene() { renderer->SetupObjects(*this); }
