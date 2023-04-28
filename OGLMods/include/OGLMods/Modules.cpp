#include "oglpch.h"
#include "Modules.h"

using namespace Bored::OGL;

#define OGL_MAX_VERTICES 1000
#define OGL_MAX_INDICES 100000

Renderer::Renderer() {
  if (!glfwInit()) {
    std::cout << "Cannot initialize glfw" << std::endl;
    glfwTerminate();
  }
}
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
  GLenum severity, GLsizei length,
  const char* message, const void* userParam);
void Renderer::OnSetup() {
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  /* Setup GLFW */
  if (!window) {
    std::cout << "Cannot create window" << std::endl;
    glfwTerminate();
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  /* Setup Glad */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
  }

  int ma, mi;
  glGetIntegerv(GL_MAJOR_VERSION, &ma);
  glGetIntegerv(GL_MINOR_VERSION, &mi);
  std::cout << "OpengGL version: " << ma << "." << mi << std::endl;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Setup debug context */
  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    // initialize debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
      GL_TRUE);
  }

  /* Setup ImGui */
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  meshShader = Shader("mesh.vert", "mesh.frag");
  testShader = Shader("test.vert", "test.frag");

  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);

  glGenBuffers(1, &mPosVbo);
  glGenBuffers(1, &mUVsVbo);
  glGenBuffers(1, &mNormVbo);
  glGenBuffers(1, &mIbo);

  glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * OGL_MAX_VERTICES, nullptr,
    GL_DYNAMIC_DRAW);
  glEnableVertexArrayAttrib(mVao, 0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * OGL_MAX_INDICES,
    nullptr, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, mUVsVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * OGL_MAX_VERTICES, nullptr,
    GL_DYNAMIC_DRAW);
  glEnableVertexArrayAttrib(mVao, 1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  glBindBuffer(GL_ARRAY_BUFFER, mNormVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * OGL_MAX_VERTICES, nullptr,
    GL_DYNAMIC_DRAW); glEnableVertexArrayAttrib(mVao, 2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

  unsigned char d[] = { 
    0xff, 0xff, 0xff, 0xff, // white
    0xff, 0x00, 0x00, 0xff // red
  };

  unsigned char d2[] = {
    0xff, 0xff, 0xff, 0xff,
    0x0f, 0xff, 0xff, 0xff,
    0x0f, 0x00, 0x00, 0xff,
    0xff, 0xff, 0xff, 0xff,
  };
  std::shared_ptr<Bored::Render::Texture> t = 
    Bored::Render::TextureFactory::Load("white", 1, 1, 4, d);
  white = Texture(t);

  t = Bored::Render::TextureFactory::Load("red", 1, 1, 4, &d[4]);
  red = Texture(t);

  t = Bored::Render::TextureFactory::Load("mixed", 2, 2, 4, d2);
  mixed = Texture(t);

  t = Bored::Render::TextureFactory::Load("./res/textures/linhtinh.png");
  img = Texture(t);

  light = actorManager->Create<Bored::Actor>();
  auto& l = light->AddComponent<Bored::Render::Light>();
  auto& mod = light->AddComponent<Bored::Render::Model>(
    Bored::Helper::Load("./res/models/pawn.gltf"));
  auto& trans = light->Get<Bored::Transform>();
  trans.pos = { 3.5f, 3.5f, 25.0f };
  trans.scale = { 0.1f, 0.1f, 0.1f };
  l.color = { 1.0f, 1.0f, 1.0f };
  l.ambient = 0.05f;
  l.diffuse = 0.35f;
  l.specular = 0.1f;

  img.Bind(testShader, "mat.diffuse", 0);
  mixed.Bind(testShader, "mat.specular", 1);
  testShader.SetUniform1f("mat.opacity", 1.0f);
  testShader.SetUniform1i("mat.shiny", 32);

  t.reset();

  // Create OGL Model for all actors
  auto v = actorManager->Get<Bored::Render::Model>();
  for (auto& id : v)
  {
    auto& mod = v.get<Bored::Render::Model>(id);
    Bored::OGL::Model m(mod);
    actorManager->AddComponent<Bored::OGL::Model>(id, mod);
  }
}

bool Renderer::OnTick(double dt) {
  /* ========== Data preparation =========== */
  // Get current camera
  if (!active_cam)
  {
    std::cout << "[Warning]: No camera is set. Using default one." << std::endl;
    active_cam = actorManager->Create<Bored::Actor>();
    auto& c = active_cam->AddComponent<Bored::Camera>();
    auto& t = active_cam->Get<Bored::Transform>();
    t.pos = { -7.0f, 0.0f, 25.0f };
    c.pitch = -75.0f;
  }
  // Set camera info
  auto& camTrans = active_cam->Get<Bored::Transform>();
  auto& camCam = active_cam->Get<Bored::Camera>();
  auto dir = camCam.GetDir();
  testShader.SetUniform3f("cam.pos", camTrans.pos.x, camTrans.pos.y, camTrans.pos.z);
  testShader.SetUniform3f("cam.dir", dir.x, dir.y, dir.z);
  testShader.SetUniform3f("cam.up", camCam.up.x, camCam.up.y, camCam.up.z);

  // Get all light sources
  auto& l = light->Get<Bored::Render::Light>();
  auto& lTrans = light->Get<Bored::Transform>();
  // Set light sources
  testShader.SetUniform3f("light.color", l.color.r, l.color.g, l.color.b);
  testShader.SetUniform3f("light.pos", lTrans.pos.x, lTrans.pos.y, lTrans.pos.z);
  testShader.SetUniform1f("light.ambient", l.ambient);
  testShader.SetUniform1f("light.diffuse", l.diffuse);
  testShader.SetUniform1f("light.specular", l.specular);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  // Get all models
  auto v = actorManager->Get<Bored::OGL::Model, Bored::Transform>();

  /* ========= Render =========== */
  auto view = active_cam->Get<Bored::Camera>().GetViewMat(camTrans.pos);
  auto proj = GetProjMat();

  testShader.SetUniformMatrix4fv("mvp.View", glm::value_ptr(view));
  testShader.SetUniformMatrix4fv("mvp.Proj", glm::value_ptr(proj));

  testShader.Activate();
  for (auto& x : v)
  {
    auto& t = v.get<Bored::Transform>(x);
    auto& m = v.get<Bored::OGL::Model>(x);
    //auto model = t.GetMat();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, t.pos);
    model = glm::rotate(model, glm::radians(t.rotation.x), { 1.0f, 0.0f, 0.0f });
    model = glm::rotate(model, glm::radians(t.rotation.y), { 0.0f, 1.0f, 0.0f });
    model = glm::rotate(model, glm::radians(t.rotation.z), { 0.0f, 0.0f, 1.0f });
    model = glm::scale(model, t.scale);
    testShader.SetUniformMatrix4fv("mvp.Model", glm::value_ptr(model));
    Render(m, testShader);
  }

  // Render a rectangle
  Bored::Render::Mesh me;
  Bored::OGL::Material ma;
  std::vector<glm::vec3> pos = {
    {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, 
    {1.0f, 1.0f, 0.0}, {0.0f, 1.0f, 0.0f} };
  std::vector<glm::vec2> uvs = 
    { {0.0, 0.0}, {1.0, 0.0}, {1.0, 1.0}, {0.0, 1.0} };
  std::vector<glm::vec3> norms = {
      {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0} };
  std::vector<unsigned int> indices = { 0, 1, 2, 0, 2, 3 };

  me.pos = pos;
  me.uvs = uvs;
  me.norms = norms;
  me.indices = indices;
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, { -0.5f, -0.5f, -1.0f });
  model = glm::scale(model, { 8.0f, 8.0f, 8.0f });
  testShader.SetUniformMatrix4fv("mvp.Model", glm::value_ptr(model));
  Render(me, ma, testShader);

  /* ========== ImGui =========== */
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  gamemode->OnImGui();
  actorManager->OnImGui();
  renderer->OnImGui();
  for (auto mod : addons)
    mod->OnImGui();

  ImGui::Render();
  glViewport(0, 0, width, height);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
  return false;
}

void Renderer::OnImGui() {
  ImGui::Begin("Window size");
  ImGui::Text("Width: %d", width);
  ImGui::Text("Height: %d", height);
  ImGui::End();

  auto& camTrans = active_cam->Get<Bored::Transform>();
  auto& camCam = active_cam->Get<Bored::Camera>();
  ImGui::Begin("Active cam");
  ImGui::DragFloat3("Pos", (float*)&camTrans.pos, 0.1f, -100.0f, 100.0f, "%.2f");
  ImGui::DragFloat("Yaw", &camCam.yaw, 0.1f, -180.0f, 180.0f, "%.1f");
  ImGui::DragFloat("Pitch", &camCam.pitch, 0.1f, -180.0f, 180.0f, "%.1f");
  ImGui::End();

  auto& l = light->Get<Bored::Render::Light>();
  auto& lTrans = light->Get<Bored::Transform>();
  ImGui::Begin("Light");
  ImGui::DragFloat3("Pos", (float*)&lTrans.pos, 0.1f);
  ImGui::DragFloat3("Color", (float*)&l.color, 0.001f, 0.0f, 1.0f, "%.3f");
  ImGui::DragFloat("Ambient", &l.ambient, 0.001f, 0.0f, 1.0f, "%.3f");
  ImGui::DragFloat("Diffuse", &l.diffuse, 0.001f, 0.0f, 1.0f, "%.3f");
  ImGui::DragFloat("Specular", &l.specular, 0.001f, 0.0f, 1.0f, "%.3f");
  ImGui::End();
}

void Renderer::OnStop() {
  // Clear Imgui stuff
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Clear member buffers of this class
  glDeleteVertexArrays(1, &mVao);
  glDeleteBuffers(1, &mPosVbo);
  glDeleteBuffers(1, &mUVsVbo);
  glDeleteBuffers(1, &mNormVbo);
  glDeleteBuffers(1, &mIbo);
  meshShader.Delete();
  testShader.Delete();
  white.Delete();
  red.Delete();
  mixed.Delete();
  img.Delete();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void Renderer::Render(const Bored::OGL::Model& m, Shader& shader) {
  for (auto& [mesh, mat] : m.renderables) {
    Render(*mesh, *mat, shader);
  }
}

void Renderer::Render(Bored::Render::Mesh& me,
  Bored::OGL::Material& ma, Shader& shader) {
  // Shader is already activated, no need to activate here 
  if (me.indices.size() == 0) {
    std::cout << "[Wawrning]: Mesh '" << me.name
      << "' does not have vertex indices" << std::endl;
  }

  if (me.pos.size() == 0) {
    std::cout << "[Warning]: Mesh '" << me.name << "' missing vertex data"
      << std::endl;
  }

  glBindVertexArray(mVao);

  // Subdata into vbos
  glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(me.pos[0]) * me.pos.size(),
    &me.pos[0]);

  glBindBuffer(GL_ARRAY_BUFFER, mUVsVbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(me.uvs[0]) * me.uvs.size(),
    &me.uvs[0]);

  glBindBuffer(GL_ARRAY_BUFFER, mNormVbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(me.norms[0]) * me.norms.size(),
    &me.norms[0]);

  // Subdata into ibo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
    sizeof(me.indices[0]) * me.indices.size(), &me.indices[0]);

  // Push material data
  if (!ma.diffuse)
    ma.diffuse = std::make_shared<Bored::OGL::Texture>(
      Bored::Render::TextureFactory::Load("white"));
  if (!ma.specular)
    ma.specular = std::make_shared<Bored::OGL::Texture>(
      Bored::Render::TextureFactory::Load("white"));
  shader.SetUniform1f("mat.opacity", ma.opacity);
  ma.diffuse->Bind(shader, "mat.diffuse", 0);
  ma.specular->Bind(shader, "mat.specular", 1);

  // glDraw
  glDrawElements(GL_TRIANGLES, me.indices.size(), GL_UNSIGNED_INT, 0);
}

void Input::OnSetup() {
  std::shared_ptr<OGL::Renderer> r =
    std::dynamic_pointer_cast<OGL::Renderer>(renderer);
  if (!r) {
    std::cout << "[Critical]: OGL input needs OGL renderer to function!"
      << std::endl;
  }
  window = r->window;
}

bool Input::OnTick(double dt) {
  glfwWaitEvents();
  if (!window) {
    std::cout << "[Critical]: OGL input does not have GLFW window!"
      << std::endl;
    return true;
  }
  return glfwWindowShouldClose(window);
}

void Bored::OGL::Input::OnStop()
{
}

void Renderer::window_size_callback(GLFWwindow* window, int w, int h) {
  renderer->width = w;
  renderer->height = h;
}

void Renderer::framebuffer_size_callback(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, w, h);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
  GLenum severity, GLsizei length,
  const char* message, const void* userParam) {
  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    return;

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " << message << std::endl;

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    std::cout << "Source: API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    std::cout << "Source: Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    std::cout << "Source: Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    std::cout << "Source: Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    std::cout << "Source: Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    std::cout << "Source: Other";
    break;
  }
  std::cout << std::endl;

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    std::cout << "Type: Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::cout << "Type: Deprecated Behaviour";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::cout << "Type: Undefined Behaviour";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    std::cout << "Type: Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    std::cout << "Type: Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    std::cout << "Type: Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    std::cout << "Type: Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    std::cout << "Type: Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    std::cout << "Type: Other";
    break;
  }
  std::cout << std::endl;

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    std::cout << "Severity: high";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    std::cout << "Severity: medium";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    std::cout << "Severity: low";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    std::cout << "Severity: notification";
    break;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}
