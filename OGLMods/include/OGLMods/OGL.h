#pragma once
#ifndef OGL_MODS_H

#include "../../imports.h"
// Remember to include glad before glfw
#include "Buffers/IndexBuffer.h"
#include "Buffers/VertexBuffer.h"
#include "ShaderClass.h"

namespace Bored {
namespace OGL {
struct BasicMeshComp : public Bored::Component
{
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;
};

class Renderer : public Bored::Renderer {
 public:
  GLFWwindow* window;

  std::unique_ptr<VertexArray> vao;
  std::unique_ptr<VertexBuffer> pos_vbo;
  std::unique_ptr<VertexBuffer> uv_vbo;
  std::unique_ptr<VertexBuffer> norm_vbo;
  std::unique_ptr<IndexBuffer> ibo;

  Shader meshShader;

  Renderer() {
    if (!glfwInit()) {
      std::cout << "Cannot initialize glfw" << std::endl;
      glfwTerminate();
    }
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  }

  // Inherited via Renderer
  virtual void OnSetup() override {
    if (!window) {
      std::cout << "Cannot create window" << std::endl;
      glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
    }
    glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    vao = std::make_unique<VertexArray>();
    pos_vbo = std::make_unique<VertexBuffer>((void*)0, sizeof(float) * 1000 * 3, GL_DYNAMIC_DRAW);
    uv_vbo = std::make_unique<VertexBuffer>((void*)0, sizeof(float) * 1000 * 2, GL_DYNAMIC_DRAW);
    norm_vbo = std::make_unique<VertexBuffer>((void*)0, sizeof(float) * 1000 * 3, GL_DYNAMIC_DRAW);
    ibo = std::make_unique<IndexBuffer>(nullptr, sizeof(unsigned int) * 10000,
                                        GL_DYNAMIC_DRAW);
    meshShader = Shader("mesh.vert", "mesh.frag");

    pos_vbo->SetLayout({{"pos", Float3, GL_FALSE}});
    uv_vbo->SetLayout({{"uv", Float2, GL_FALSE}});
    norm_vbo->SetLayout({{"norm", Float3, GL_FALSE}});
    vao->AddVertexBuffer(*pos_vbo);
    vao->AddVertexBuffer(*uv_vbo);
    vao->AddVertexBuffer(*norm_vbo);
  }

  static void window_size_callback(GLFWwindow* window, int w, int h)
  {
    renderer->width = w;
    renderer->height = h;
  }

  static void framebuffer_size_callback(GLFWwindow* window, int w, int h)
  {
    glViewport(0, 0, w, h);
  }

  virtual bool OnTick(double dt) override {
    // Set camera info

    // Set light sources

    //for (auto a : actorManager->actors) {
    //    // Render the actor

    //    // Check if there is mesh
    //}

    // ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    gamemode->OnImGui();
    actorManager->OnImGui();
    renderer->OnImGui();
    for (auto mod : addons) mod->OnImGui();

    ImGui::Render();
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    return false;
  }

  virtual void OnImGui() override {
    ImGui::Begin("Window size");
    ImGui::Text("Width: %d", width);
    ImGui::Text("Height: %d", height);
    ImGui::End();
  }

  virtual void OnStop() override {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
  }
};

class Input : public Bored::Input {
 public:
  GLFWwindow* window;

  // Inherited via Input
  virtual void OnSetup() override {}

  virtual bool OnTick(double dt) override {
    glfwWaitEvents();
    return glfwWindowShouldClose(window);
  }
};

};  // namespace OGL
}  // namespace Bored

#endif  // !OGL_MODS_H
