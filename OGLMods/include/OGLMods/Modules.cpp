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
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
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

void Input::SetCursorImage(unsigned char* image, unsigned int width, unsigned int height)
{
	GLFWimage im;
	im.pixels = image;
	im.width = width;
	im.height = height;
	GLFWcursor* c = glfwCreateCursor(&im, 0, 0);
	if (c == NULL) {
		std::cout << "Oops, something went wrong when setting cursor image." << std::endl;
	}
	if (window != nullptr)
		glfwSetCursor(window, c);
	else {
		std::cout << "Fatal error: Input is not being used with GLFWWindow." << std::endl;
	}
}

void Input::EnableCursor()
{
	if (window) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Input::DisableCursor()
{
	if (window) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
}

KeyInput::Key Input::GetKey(int keyCode)
{
	switch (keyCode) {
		case GLFW_KEY_1: return KeyInput::KEY_1;
		case GLFW_KEY_2: return KeyInput::KEY_2;
		case GLFW_KEY_3: return KeyInput::KEY_3;
		case GLFW_KEY_4: return KeyInput::KEY_4;
		case GLFW_KEY_5: return KeyInput::KEY_5;
		case GLFW_KEY_6: return KeyInput::KEY_6;
		case GLFW_KEY_7: return KeyInput::KEY_7;
		case GLFW_KEY_8: return KeyInput::KEY_8;
		case GLFW_KEY_9: return KeyInput::KEY_9;
		case GLFW_KEY_0: return KeyInput::KEY_0;
		case GLFW_KEY_Q: return KeyInput::KEY_Q;
		case GLFW_KEY_W: return KeyInput::KEY_W;
		case GLFW_KEY_E: return KeyInput::KEY_E;
		case GLFW_KEY_R: return KeyInput::KEY_R;
		case GLFW_KEY_T: return KeyInput::KEY_T;
		case GLFW_KEY_Y: return KeyInput::KEY_Y;
		case GLFW_KEY_U: return KeyInput::KEY_U;
		case GLFW_KEY_I: return KeyInput::KEY_I;
		case GLFW_KEY_O: return KeyInput::KEY_O;
		case GLFW_KEY_P: return KeyInput::KEY_P;
		case GLFW_KEY_A: return KeyInput::KEY_A;
		case GLFW_KEY_S: return KeyInput::KEY_S;
		case GLFW_KEY_D: return KeyInput::KEY_D;
		case GLFW_KEY_F: return KeyInput::KEY_F;
		case GLFW_KEY_G: return KeyInput::KEY_G;
		case GLFW_KEY_H: return KeyInput::KEY_H;
		case GLFW_KEY_J: return KeyInput::KEY_J;
		case GLFW_KEY_K: return KeyInput::KEY_K;
		case GLFW_KEY_L: return KeyInput::KEY_L;
		case GLFW_KEY_Z: return KeyInput::KEY_Z;
		case GLFW_KEY_X: return KeyInput::KEY_X;
		case GLFW_KEY_C: return KeyInput::KEY_C;
		case GLFW_KEY_V: return KeyInput::KEY_V;
		case GLFW_KEY_B: return KeyInput::KEY_B;
		case GLFW_KEY_N: return KeyInput::KEY_N;
		case GLFW_KEY_M: return KeyInput::KEY_M;
		case GLFW_KEY_SPACE: return KeyInput::KEY_SPACE;
		case GLFW_KEY_LEFT_SHIFT: return KeyInput::KEY_LEFT_SHIFT;
		case GLFW_KEY_LEFT_CONTROL: return KeyInput::KEY_LEFT_CONTROL;
		// mouse
		case GLFW_MOUSE_BUTTON_1: return KeyInput::KEY_MB_1;
		case GLFW_MOUSE_BUTTON_2: return KeyInput::KEY_MB_2;
		case GLFW_MOUSE_BUTTON_3: return KeyInput::KEY_MB_3;
		case GLFW_MOUSE_BUTTON_4: return KeyInput::KEY_MB_4;
		case GLFW_MOUSE_BUTTON_5: return KeyInput::KEY_MB_5;
		case GLFW_MOUSE_BUTTON_6: return KeyInput::KEY_MB_6;
		case GLFW_MOUSE_BUTTON_7: return KeyInput::KEY_MB_7;
		case GLFW_MOUSE_BUTTON_8: return KeyInput::KEY_MB_8;
		
		// default
		default: return KeyInput::KEY_UNKNOWN;
	}
}

int Input::GetMods(int modBits)
{
	int ans = 0;
	if (modBits & GLFW_MOD_CONTROL) 
		ans |= KeyInput::CTRL;
	if (modBits & GLFW_MOD_SHIFT)
		ans |= KeyInput::SHIFT;
	if (modBits & GLFW_MOD_ALT)
		ans |= KeyInput::ALT;
	return ans;
}

KeyInput::Action Input::GetAction(int actionCode)
{
	switch (actionCode) {
	case GLFW_PRESS: return KeyInput::PRESS;
	case GLFW_REPEAT: return KeyInput::REPEAT;
	case GLFW_RELEASE: return KeyInput::RELEASE;
	default: return KeyInput::UNKNOWN;
	}
}

void Input::SetupCallbacks()
{

	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetCursorPosCallback(window, Input::MousePosCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetCursorEnterCallback(window, Input::MouseEnterCallback);
	glfwSetScrollCallback(window, Input::MouseScrollCallback);
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



void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	input->EvaluateKey(input->GetKey(key), input->GetAction(action), input->GetMods(mods), 1.0f);
}

void Input::MousePosCallback(GLFWwindow* window, double x, double y)
{
	
	input->mouseInfo.posX = x;
	input->mouseInfo.posY = y;
	input->EvaluateKey(KeyInput::MOUSE_POS_X, KeyInput::PRESS, 0, x);
	input->EvaluateKey(KeyInput::MOUSE_POS_Y, KeyInput::PRESS, 0, y);
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	
	input->EvaluateKey(input->GetKey(button), input->GetAction(action), input->GetMods(mods), 1.0f);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		//auto a = input->GetCursorHoveringActor();
		//if (a != nullptr)
		//	a->OnClick();
	}
}

void Input::MouseEnterCallback(GLFWwindow* window, int entered)
{
	if (entered) {
		input->EvaluateKey(KeyInput::MOUSE_ENTER, KeyInput::PRESS, 0, 1.0f);
		input->mouseInfo.isEntered = true;
	}
	else {
		input->EvaluateKey(KeyInput::MOUSE_LEAVE, KeyInput::PRESS, 0, 1.0f);
		input->mouseInfo.isEntered = false;
	}
}

void Input::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	input->EvaluateKey(KeyInput::MOUSE_SCROLL_X, KeyInput::PRESS, 0, x);
	input->EvaluateKey(KeyInput::MOUSE_SCROLL_Y, KeyInput::PRESS, 0, y);
	input->mouseInfo.scrollX = x;
	input->mouseInfo.scrollY = y;
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
}





