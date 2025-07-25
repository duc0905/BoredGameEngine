#include "Nodes/Camera.hpp"
#include "Nodes/Lighting.hpp"
#include "Nodes/Mesh3D.hpp"
#include "Renderer/I_Object.hpp"
#include "Renderer/OGL/Renderer.hpp"
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

  std::shared_ptr<Bored::Node> root = std::make_shared<Bored::Node>();
  scene->SetRoot(root);

  std::shared_ptr<Bored::Camera> camera =
      std::make_shared<Bored::Camera>(new Bored::Perspective(
          Bored::Perspective::GetFOV(1.0f, 2.0f), SCR_WIDTH, SCR_HEIGHT, 1.0f));
  camera->transform.translate = {0.0f, 2.0f, 4.0f};
  camera->transform.rotate.x = -glm::pi<float>() / 6;

  root->AddChild(camera);
  scene->SetActiveCamera(camera);

  std::shared_ptr<Bored::ArrayMesh> mesh;

  // Loading mesh from memory
  std::vector<glm::vec3> pos = {
      {-1.0f, -1.0f, 0.0f}, {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
  std::vector<glm::vec2> uvs = {{0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f}};
  std::vector<glm::vec3> norms = {
      {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
  std::vector<uint32_t> indices = {0, 1, 2};
  mesh = std::make_shared<Bored::ArrayMesh>();
  mesh->SubData(pos, uvs, norms, indices);
  mesh->transform.scale = {2.0f, 1.0f, 1.0f};
  mesh->transform.translate = {1.0f, -1.8f, 1.0f};
  mesh->transform.rotate.x = -glm::pi<float>() * 0.3f;

  root->AddChild(mesh);

  // Loading mesh from files
  try {
    mesh = OGL::LoadModel("res/models/kitchentable_sink_large_decorated.gltf");
    root->AddChild(mesh);

    mesh = OGL::LoadModel("res/models/chair_A.gltf");
    mesh->transform.translate = {3.5f, -0.5f, 0.0f};
    root->AddChild(mesh);

    mesh = OGL::LoadModel("res/models/cube.gltf");
    mesh->transform.translate = {-3.5f, -0.5f, 0.5f};
    root->AddChild(mesh);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  // Setting up light sources
  std::shared_ptr<Bored::DirectionalLight> dirLight =
      std::make_shared<Bored::DirectionalLight>();
  dirLight->light_color = {0.8f, 0.6f, 0.2f};
  dirLight->transform.rotate = {0.3f, 0.1f, 0.0f};
  root->AddChild(dirLight);

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
      mesh->transform.rotate.y += glm::pi<float>() * dt / 6000.0f;
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
