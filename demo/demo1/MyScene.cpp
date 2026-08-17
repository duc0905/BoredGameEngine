#include "MyScene.hpp"

#include <functional>

#include "Components/BehaviourComponent.hpp"
#include "Components/Camera.hpp"
#include "Components/InputComponent.hpp"
#include "Components/Lighting.hpp"
#include "Components/MeshComponent.hpp"
#include "Systems/BehaviourSystem.hpp"
#include "Systems/Input/IOService.hpp"
#include "Utils/AssetManager.hpp"

void CameraController::MoveCamera(Bored::InputEvent& event) {
  if (event.type == Bored::InputType::KEY_DOWN ||
      event.type == Bored::InputType::KEY_REPEAT) {
    if (event.key.keyCode == GLFW_KEY_W) {
      move_forward = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_A) {
      move_left = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_S) {
      move_backward = true;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_D) {
      move_right = true;
      event.handled = true;
    }
  }

  if (event.type == Bored::InputType::KEY_UP) {
    if (event.key.keyCode == GLFW_KEY_W) {
      move_forward = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_A) {
      move_left = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_S) {
      move_backward = false;
      event.handled = true;
    }

    if (event.key.keyCode == GLFW_KEY_D) {
      move_right = false;
      event.handled = true;
    }
  }

  if (event.type == Bored::InputType::KEY_DOWN &&
      event.key.keyCode == GLFW_KEY_ESCAPE) {
    if (is_mouse_hidden) {
      obj->scene.context.io->SetCursorMode(Bored::CursorMode::VISIBLE);
      is_mouse_hidden = false;
    } else {
      obj->scene.context.io->SetCursorMode(Bored::CursorMode::DISABLED);
      is_mouse_hidden = true;
    }
  }
}
void CameraController::RotateCamera(Bored::InputEvent& event) {
  if (is_mouse_hidden) {
    if (event.type == Bored::InputType::MOUSE_MOVE) {
      dx += event.mouseMove.dx;
      dy += event.mouseMove.dy;

      // glm::vec3 rotation{-event.mouseMove.dy, -event.mouseMove.dx, 0.0f};
      // rotation *= dt * sensivity;
      // node->transform.rotate += rotation;
      event.handled = true;
    }
  }
}

void CameraController::OnUpdate(float dt) {
  glm::vec3 direction = glm::vec3(0.0f);

  if (move_forward) {
    direction += glm::vec3{0.0f, 0.0f, -1.0f};
  }

  if (move_backward) {
    direction += glm::vec3{0.0f, 0.0f, 1.0f};
  }

  if (move_left) {
    direction += glm::vec3{-1.0f, 0.0f, 0.0f};
  }

  if (move_right) {
    direction += glm::vec3{1.0f, 0.0f, 0.0f};
  }

  // if (event.handled) {
  //   direction =
  //       node->transform.GetTransformMatrix() * glm::vec4(direction, 0.0f);
  //   direction = glm::normalize(direction);
  //   node->transform.translate += direction * velocity * (float)dt;
  // }

  direction = obj->transform.GetTransformMatrix() * glm::vec4(direction, 0.0f);
  obj->transform.translate += direction * velocity * (float)dt;

  glm::vec3 rotation{-dy, -dx, 0.0f};
  rotation *= dt * sensivity;
  obj->transform.rotate += rotation;

  dx = dy = 0.0f;
}

MyScene::MyScene() {
  // Setup window system
  context.io = std::make_shared<Bored::IOService>(SCR_WIDTH, SCR_HEIGHT);
  renderer = std::make_shared<OGL::Renderer>(*context.io);
  systems.push_back(renderer);

  // Setup input system
  input = std::make_shared<Bored::InputSystem>(*context.io);
  systems.push_back(input);

  // Using Behaviour
  auto behaviour = std::make_shared<Bored::BehaviourSystem>();
  systems.push_back(behaviour);
}

void MyScene::BuildScene() {
  // TODO: Serialize/deserialize scene

  // Input context
  auto [ctx_idx, ctx] = input->CreateContext();
  input->SwitchContext(ctx_idx);

  // Building scene
  std::shared_ptr<Bored::Object> root = CreateNode();
  SetRoot(root);

  std::shared_ptr<Bored::Object> camera_node = CreateNode();
  camera_node->AddComponent<Bored::CameraComponent>(new Bored::Perspective(
      Bored::Perspective::GetFOV(1.0f, 2.0f), SCR_WIDTH, SCR_HEIGHT, 1.0f));
  Bored::TransformComponent& camera_transform =
      camera_node->GetComponent<Bored::TransformComponent>();
  Bored::BehaviourComponent& camera_behaviour =
      camera_node->AddComponent<Bored::BehaviourComponent>();
  std::shared_ptr<CameraController> camera_controller =
      camera_node->CreateBehaviour<CameraController>();
  camera_behaviour.behaviour = camera_controller;

  ctx.RegisterInput(Bored::InputType::MOUSE_MOVE, "Rotate camera");
  ctx.RegisterInput(Bored::InputType::KEY_DOWN, "Move camera");
  ctx.RegisterInput(Bored::InputType::KEY_REPEAT, "Move camera");
  ctx.RegisterInput(Bored::InputType::KEY_UP, "Move camera");

  ctx.RegisterHandler(
      "Move camera", std::bind(&CameraController::MoveCamera,
                               camera_controller.get(), std::placeholders::_1));

  ctx.RegisterHandler(
      "Rotate camera",
      std::bind(&CameraController::RotateCamera, camera_controller.get(),
                std::placeholders::_1));

  camera_transform.translate = {0.0f, 2.0f, 4.0f};
  camera_transform.rotate.x = -glm::pi<float>() / 6;

  root->AddChild(camera_node);
  SetActiveCamera(camera_node);

  std::shared_ptr<Bored::Object> triangle_node = CreateNode();
  Bored::MeshComponent& triangle_mesh_comp =
      triangle_node->AddComponent<Bored::MeshComponent>();
  triangle_mesh_comp.mesh = std::make_shared<Bored::ArrayMesh>();

  // Loading mesh from memory
  std::vector<glm::vec3> pos = {
      {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
  std::vector<glm::vec2> uvs = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}};
  std::vector<glm::vec3> norms = {
      {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
  std::vector<uint32_t> indices = {0, 1, 2};
  triangle_mesh_comp.mesh->SubData(pos, uvs, norms, indices);

  triangle_node->transform.scale = {2.0f, 1.0f, 1.0f};
  triangle_node->transform.translate = {1.0f, -1.8f, 1.0f};
  triangle_node->transform.rotate.x = -glm::pi<float>() * 0.3f;

  root->AddChild(triangle_node);
  auto& am = Bored::AssetManager::GetInstance();

  // Loading mesh from files
  try {
    std::shared_ptr<Bored::Object> kitchen_table_node = CreateNode();
    Bored::MeshComponent& kitchen_table_mesh_comp =
        kitchen_table_node->AddComponent<Bored::MeshComponent>();
    kitchen_table_mesh_comp =
        *am.LoadModel("res/models/kitchentable_sink_large_decorated.gltf");
    root->AddChild(kitchen_table_node);

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  try {
    std::shared_ptr<Bored::Object> chair_node = CreateNode();
    Bored::MeshComponent& chair_mesh_comp =
        chair_node->AddComponent<Bored::MeshComponent>();
    chair_mesh_comp = *am.LoadModel("res/models/chair_A.gltf");
    chair_node->transform.translate = {3.5f, -0.5f, 0.0f};
    root->AddChild(chair_node);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  try {
    std::shared_ptr<Bored::Object> cube_node = CreateNode();
    Bored::MeshComponent& cube_mesh_comp =
        cube_node->AddComponent<Bored::MeshComponent>();
    cube_mesh_comp = *am.LoadModel("res/models/cube.gltf");
    cube_node->transform.translate = {-3.5f, -0.5f, 0.5f};
    root->AddChild(cube_node);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  // Setting up light sources
  std::shared_ptr<Bored::Object> dir_light_node = CreateNode();
  Bored::DirectionalLight& dir_light_comp =
      dir_light_node->AddComponent<Bored::DirectionalLight>();
  dir_light_comp.light_color = {0.8f, 0.6f, 0.2f};
  dir_light_node->transform.rotate = {0.3f, 0.1f, 0.0f};
  root->AddChild(dir_light_node);
}

void MyScene::AfterBuildScene() {
  // Post scene building
  // TODO: Consider making this function for all systems
  renderer->SetupObjects(*this);
}
