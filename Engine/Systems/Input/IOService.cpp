#include "IOService.hpp"
#include <GLFW/glfw3.h>

namespace Bored {
IOService::IOService(int width, int height) {
  if (!glfwInit()) {
    throw std::runtime_error("GLFW initialization failed");
  }

  if (m_debug) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  }

  // Anti aliasing
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(width, height, "OpenGL Window", NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("GLFW window creation failed");
  }

  glfwMakeContextCurrent(window);

  // Magic stuff
  glfwSetWindowUserPointer(window, this);

  // Set callback handlers
  glfwSetKeyCallback(window, &IOService::KeyCallback);
  glfwSetCursorPosCallback(window, &IOService::CursorPosCallback);
  glfwSetMouseButtonCallback(window, &IOService::MouseButtonCallback);
  glfwSetFramebufferSizeCallback(window, &IOService::FrameBufferSizeCallback);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("GLAD initialization failed");
  }

  if (m_debug) {
    // Print the OpenGL version
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "[Info]: OpenGL version " << major << "." << minor
              << std::endl;

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT)) {
      std::cout << "[Warning]: Failed to initialize OpenGL debug context"
                << std::endl;
    } else {
      std::cout << "[Info]: Enabling OpenGL Debug output" << std::endl;
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(IOService::DebugOutputCallback, this);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                            nullptr, GL_TRUE);
    }
  }

  // Enable anti-aliasing
  glEnable(GL_MULTISAMPLE);

  // Use byte alignment
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Create a VAO for rendering the texture covers the whole screen
  GLuint VBO, EBO;

  // clang-format off
    float vertices[] = {
        // Position               UV
        -0.8f, -0.8f, 0.2f, 1.0f, 0.0f, 0.0f,
        -0.8f,  0.8f, 0.2f, 1.0f, 0.0f, 1.0f,
         0.8f,  0.8f, 0.2f, 1.0f, 1.0f, 1.0f,
         0.8f, -0.8f, 0.2f, 1.0f, 1.0f, 0.0f,
    };
  // clang-format on
  unsigned int indices[] = {0, 1, 2, 0, 2, 3};

  glGenVertexArrays(1, &m_screenVao);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(m_screenVao);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // UV
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(4 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  m_renderTexture = std::make_unique<OGL_Texture2D>();
  m_screenShader = std::make_unique<Shader>("res/shaders/simple.vert",
                                            "res/shaders/simple_texture.frag");
}

void IOService::SetCursorMode(CursorMode mode) {
  switch (mode) {
  case CursorMode::VISIBLE:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case CursorMode::CAPTURED:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    break;
  case CursorMode::HIDDEN:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    break;
  case CursorMode::DISABLED:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    break;
  }
}

void IOService::PollEvents() { glfwPollEvents(); }

bool IOService::ShouldStop() const { return glfwWindowShouldClose(window); }

std::pair<int, int> IOService::GetCursorPos() const {
  double x, y;
  glfwGetCursorPos(window, &x, &y);
  return {(int)x, (int)y};
}

bool IOService::IsKeyPressed(int key) const { return glfwGetKey(window, key); }

std::pair<int, int> IOService::GetFrameBufferSize() const {
  std::pair<int, int> size;
  glfwGetFramebufferSize(window, &size.first, &size.second);
  return size;
}

void IOService::Render(std::shared_ptr<I_Texture2D> texture) {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!texture) {
    return;
  }

  m_renderTexture->WriteData(texture->GetData(), texture->GetSize(),
                             texture->GetBPP());

  m_screenShader->Use();
  glBindTexture(GL_TEXTURE_2D, m_renderTexture->m_texId);
  glBindVertexArray(m_screenVao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
}

void IOService::SetKeyHandler(std::function<void(int, int, int)> handler) {
  key_callback = handler;
}

void IOService::SetCursorPosHandler(std::function<void(int, int)> handler) {
  cursor_pos_callback = handler;
}

void IOService::SetFrameBufferSizeHandler(
    std::function<void(int, int)> handler) {
  frame_buffer_size_callback = handler;
}

void IOService::SetMouseButtonHandler(
    std::function<void(int, int, int)> handler) {
  mouse_button_callback = handler;
}

void IOService::KeyCallback(GLFWwindow *window, int key, int scancode,
                            int action, int mods) {
  IOService *this_ = static_cast<IOService *>(glfwGetWindowUserPointer(window));

  if (this_->key_callback)
    this_->key_callback(key, action, mods);
}

void IOService::CursorPosCallback(GLFWwindow *window, double x, double y) {
  IOService *this_ = static_cast<IOService *>(glfwGetWindowUserPointer(window));

  if (this_->cursor_pos_callback)
    this_->cursor_pos_callback((int)x, (int)y);
}

void IOService::FrameBufferSizeCallback(GLFWwindow *window, int width,
                                        int height) {
  IOService *this_ = static_cast<IOService *>(glfwGetWindowUserPointer(window));

  if (this_->frame_buffer_size_callback)
    this_->frame_buffer_size_callback(width, height);
}

void IOService::MouseButtonCallback(GLFWwindow *window, int key, int action,
                                    int mods) {
  IOService *this_ = static_cast<IOService *>(glfwGetWindowUserPointer(window));

  if (this_->mouse_button_callback)
    this_->mouse_button_callback(key, action, mods);
}

void IOService::HandleDebugMessage(GLenum source, GLenum type, unsigned int id,
                                   GLenum severity, GLsizei length,
                                   const char *message) {
  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    return;

  std::cout << "";

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    std::cout << "[Error]";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    std::cout << "[Error]";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    std::cout << "[Warning]";
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    std::cout << "[Info]";
    break;
  }

  std::cout << ": OpenGL (" << id << "): " << message << std::endl;

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    std::cout << "\tSource: API";
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    std::cout << "\tSource: Window System";
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    std::cout << "\tSource: Shader Compiler";
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    std::cout << "\tSource: Third Party";
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    std::cout << "\tSource: Application";
    break;
  case GL_DEBUG_SOURCE_OTHER:
    std::cout << "\tSource: Other";
    break;
  }
  std::cout << std::endl;

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    std::cout << "\tType: Error";
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::cout << "\tType: Deprecated Behaviour";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::cout << "\tType: Undefined Behaviour";
    break;
  case GL_DEBUG_TYPE_PORTABILITY:
    std::cout << "\tType: Portability";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    std::cout << "\tType: Performance";
    break;
  case GL_DEBUG_TYPE_MARKER:
    std::cout << "\tType: Marker";
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    std::cout << "\tType: Push Group";
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    std::cout << "\tType: Pop Group";
    break;
  case GL_DEBUG_TYPE_OTHER:
    std::cout << "\tType: Other";
    break;
  }

  std::cout << std::endl;
}

void IOService::DebugOutputCallback(GLenum source, GLenum type, unsigned int id,
                                    GLenum severity, GLsizei length,
                                    const char *message,
                                    const void *userParam) {
  IOService *_this =
      reinterpret_cast<IOService *>(const_cast<void *>(userParam));
  if (_this) {
    _this->HandleDebugMessage(source, type, id, severity, length, message);
  } else {
    std::cout << "[Warning]: Unknown userParam passed in debug callback"
              << std::endl;
  }
}

void IOService::HandleGLFWError(int error, const char *description) {
  std::cout << "[Error]: GLFW (" << error << "): " << description << std::endl;
}
} // namespace Bored
