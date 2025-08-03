#include "Components/Camera.hpp"
#include "Components/Lighting.hpp"
#include "Components/Mesh3D.hpp"
#include "Components/MeshComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Scene/Node.hpp"
#include "Systems/Renderer/OGL/Renderer.hpp"
#include "Window/Window.hpp"
#include <chrono>
#include <glm/ext/scalar_constants.hpp>
#include <memory>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

int main() {
  Window window(SCR_WIDTH, SCR_HEIGHT);
  OGL::Renderer renderer(

      SCR_WIDTH, SCR_HEIGHT);
  window.AddFrameBufferSizeListener(&renderer);

  // Scene setup
  std::shared_ptr<Bored::Scene> scene = std::make_shared<Bored::Scene>();

  std::shared_ptr<Bored::Node> root = scene->CreateNode();
  scene->SetRoot(root);

  std::shared_ptr<Bored::Node> camera_node = scene->CreateNode();
  camera_node->registry.emplace<Bored::CameraComponent>(
      camera_node->id,
      new Bored::Perspective(Bored::Perspective::GetFOV(1.0f, 2.0f), SCR_WIDTH,
                             SCR_HEIGHT, 1.0f));
  Bored::TransformComponent &camera_transform =
      camera_node->registry.get<Bored::TransformComponent>(camera_node->id);
  camera_transform.translate = {0.0f, 2.0f, 4.0f};
  camera_transform.rotate.x = -glm::pi<float>() / 6;

  root->AddChild(camera_node);
  scene->SetActiveCamera(camera_node);

  std::shared_ptr<Bored::Node> triangle_node = scene->CreateNode();
  Bored::MeshComponent &triangle_mesh_comp =
      triangle_node->registry.emplace<Bored::MeshComponent>(triangle_node->id);
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
    std::shared_ptr<Bored::Node> kitchen_table_node = scene->CreateNode();
    Bored::MeshComponent &kitchen_table_mesh_comp =
        kitchen_table_node->registry.emplace<Bored::MeshComponent>(
            kitchen_table_node->id);
    kitchen_table_mesh_comp.mesh =
        OGL::LoadModel("res/models/kitchentable_sink_large_decorated.gltf");
    root->AddChild(kitchen_table_node);

    std::shared_ptr<Bored::Node> chair_node = scene->CreateNode();
    Bored::MeshComponent &chair_mesh_comp =
        chair_node->registry.emplace<Bored::MeshComponent>(chair_node->id);
    chair_mesh_comp.mesh = OGL::LoadModel("res/models/chair_A.gltf");
    chair_node->transform.translate = {3.5f, -0.5f, 0.0f};
    root->AddChild(chair_node);

    std::shared_ptr<Bored::Node> cube_node = scene->CreateNode();
    Bored::MeshComponent &cube_mesh_comp =
        cube_node->registry.emplace<Bored::MeshComponent>(cube_node->id);
    cube_mesh_comp.mesh = OGL::LoadModel("res/models/cube.gltf");
    cube_node->transform.translate = {-3.5f, -0.5f, 0.5f};
    root->AddChild(cube_node);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  // Setting up light sources
  std::shared_ptr<Bored::Node> dir_light_node = scene->CreateNode();
  Bored::DirectionalLight &dir_light_comp =
      dir_light_node->registry.emplace<Bored::DirectionalLight>(
          dir_light_node->id);
  dir_light_comp.light_color = {0.8f, 0.6f, 0.2f};
  dir_light_node->transform.rotate = {0.3f, 0.1f, 0.0f};
  root->AddChild(dir_light_node);

  renderer.SetupObjects(scene);
  renderer.SetActiveScene(scene);

  bool running = true;
  std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
  // Main loop
  while (running) {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();

    // Elapsed time since last frame in milisecond
    long long dt =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
            .count();

    if (dt >= 1000.0f / 60.0f) {
      triangle_node->transform.rotate.y += glm::pi<float>() * dt / 6000.0f;
      // Render here
      auto tex = renderer.Render();
      window.Render(tex);

      prev = now;
    }

    running &= !window.ShouldStop();

    window.WaitEvents(0.0016f);
  }

  return 0;
}
