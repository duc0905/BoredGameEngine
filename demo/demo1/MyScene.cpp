#include "MyScene.hpp"

#include "Components/Camera.hpp"
#include "Components/Lighting.hpp"
#include "Components/MeshComponent.hpp"
#include "Systems/Input/InputSystem.hpp"

void CameraController::OnInput(double dt, Bored::InputEvent &event,
                               std::shared_ptr<Bored::Node> node) {
  if (event.type == Bored::InputType::KEY_DOWN ||
      event.type == Bored::InputType::KEY_REPEAT) {
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

    if (event.handled) {
      direction =
          node->transform.GetTransformMatrix() * glm::vec4(direction, 0.0f);
      direction = glm::normalize(direction);
      node->transform.translate += direction * velocity * (float)dt;
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
      node->scene.context.input_service->SetCursorMode(
          Bored::CursorMode::VISIBLE);
      is_mouse_hidden = false;
    } else {
      node->scene.context.input_service->SetCursorMode(
          Bored::CursorMode::DISABLED);
      is_mouse_hidden = true;
    }
  }
}

MyScene::MyScene() {
  // Setup window system
  window = std::make_shared<Bored::WindowService>(SCR_WIDTH, SCR_HEIGHT);
  renderer = std::make_shared<OGL::Renderer>(*window);
  window->AddFrameBufferSizeListener(renderer.get());
  systems.push_back(renderer);
  // window->renderer = renderer;
  // scene->systems.push_back(window);

  // Setup input system
  std::shared_ptr<Bored::Input> input =
      std::make_shared<Bored::Input>(*window->input_service);
  systems.push_back(input);

  // Populate the services into scene context
  context.window_service = window.get();
  context.input_service = window->input_service.get();
}

void MyScene::BuildScene() {
  // TODO: Serialize/deserialize scene

  // Building scene
  std::shared_ptr<Bored::Node> root = CreateNode();
  SetRoot(root);

  std::shared_ptr<Bored::Node> camera_node = CreateNode();
  camera_node->AddComponent<Bored::CameraComponent>(new Bored::Perspective(
      Bored::Perspective::GetFOV(1.0f, 2.0f), SCR_WIDTH, SCR_HEIGHT, 1.0f));
  Bored::TransformComponent &camera_transform =
      camera_node->GetComponent<Bored::TransformComponent>();
  Bored::InputComponent &camera_input =
      camera_node->AddComponent<Bored::InputComponent>();
  camera_input.input_handler = std::make_shared<CameraController>();

  camera_transform.translate = {0.0f, 2.0f, 4.0f};
  camera_transform.rotate.x = -glm::pi<float>() / 6;

  root->AddChild(camera_node);
  SetActiveCamera(camera_node);

  std::shared_ptr<Bored::Node> triangle_node = CreateNode();
  Bored::MeshComponent &triangle_mesh_comp =
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

  // Loading mesh from files
  try {
    std::shared_ptr<Bored::Node> kitchen_table_node = CreateNode();
    Bored::MeshComponent &kitchen_table_mesh_comp =
        kitchen_table_node->AddComponent<Bored::MeshComponent>();
    kitchen_table_mesh_comp.mesh =
        OGL::LoadModel("res/models/kitchentable_sink_large_decorated.gltf");
    root->AddChild(kitchen_table_node);

    std::shared_ptr<Bored::Node> chair_node = CreateNode();
    Bored::MeshComponent &chair_mesh_comp =
        chair_node->AddComponent<Bored::MeshComponent>();
    chair_mesh_comp.mesh = OGL::LoadModel("res/models/chair_A.gltf");
    chair_node->transform.translate = {3.5f, -0.5f, 0.0f};
    root->AddChild(chair_node);

    std::shared_ptr<Bored::Node> cube_node = CreateNode();
    Bored::MeshComponent &cube_mesh_comp =
        cube_node->AddComponent<Bored::MeshComponent>();
    cube_mesh_comp.mesh = OGL::LoadModel("res/models/cube.gltf");
    cube_node->transform.translate = {-3.5f, -0.5f, 0.5f};
    root->AddChild(cube_node);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  // Setting up light sources
  std::shared_ptr<Bored::Node> dir_light_node = CreateNode();
  Bored::DirectionalLight &dir_light_comp =
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
