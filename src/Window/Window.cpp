#include "Window.hpp"
#include "Listeners.hpp"

#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>

Window::Window(const int &w, const int &h) : m_width(w), m_height(h) {
  // Initialize OpenGL context and other setup here
  // Initialize GLFW
  if (!glfwInit()) {
    std::cout << "[Error]: Failed to initialize GLFW" << std::endl;
    throw std::runtime_error("GLFW initialization failed");
  }

  if (m_debug) {
    std::cout << "[Info]: Running in debug mode" << std::endl;
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);

  // Create a windowed mode window and its OpenGL context
  m_window = glfwCreateWindow(m_width, m_height, "OpenGL Window", NULL, NULL);
  if (!m_window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    throw std::runtime_error("GLFW window creation failed");
  }

  // Make the window's context current
  glfwMakeContextCurrent(m_window);

  // Match glfw refresh rate with monitor refresh rate
  glfwSwapInterval(1);

  // To get back this in GLFW callbacks
  glfwSetWindowUserPointer(m_window, this);

  // Window size callback
  glfwSetFramebufferSizeCallback(m_window, &Window::FrameBufferSizeCallback);

  m_prevKeyCallback = glfwSetKeyCallback(m_window, &Window::KeyCallback);

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "[Error]: Failed to initialize GLAD" << std::endl;
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
      glDebugMessageCallback(Window::DebugOutputCallback, this);
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

Window::~Window() {
  // Cleanup code here
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

bool Window::ShouldStop() const { return glfwWindowShouldClose(m_window); }

// void Window::Run() {
//   std::chrono::steady_clock::time_point prev =
//   std::chrono::steady_clock::now();
//
//   // Main loop
//   while (!glfwWindowShouldClose(m_window)) {
//     std::chrono::steady_clock::time_point now =
//         std::chrono::steady_clock::now();
//
//     // Elapsed time since last frame in milisecond
//     long long dt =
//         std::chrono::duration_cast<std::chrono::milliseconds>(now - prev)
//             .count();
//
//     if (dt >= 160) { // 6 FPS
//       // Render here
//       glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to black
//       glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
//
//       Render();
//
//       glfwSwapBuffers(m_window); // Swap front and back buffers
//
//       prev = now;
//     }
//
//     // Wait for atmost 2 seconds
//     glfwWaitEventsTimeout(2.0f); // Poll for and process events
//   }
// }

void Window::Render(std::shared_ptr<I_Texture2D> texture) {
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

  glfwSwapBuffers(m_window); // Swap front and back buffers
}

void Window::WaitEvents(float timeout) const { glfwWaitEventsTimeout(timeout); }

void Window::PollEvents() const { glfwPollEvents(); }

void Window::AddFrameBufferSizeListener(FrameBufferSizeListener *listener) {
  if (listener != nullptr) {
    m_fbsListeners.push_back(listener);
  }
}

void Window::HandleDebugMessage(GLenum source, GLenum type, unsigned int id,
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

void Window::DebugOutputCallback(GLenum source, GLenum type, unsigned int id,
                                 GLenum severity, GLsizei length,
                                 const char *message, const void *userParam) {
  Window *_this = reinterpret_cast<Window *>(const_cast<void *>(userParam));
  if (_this) {
    _this->HandleDebugMessage(source, type, id, severity, length, message);
  } else {
    std::cout << "[Warning]: Unknown userParam passed in debug callback"
              << std::endl;
  }
}

void Window::FrameBufferSizeCallback(GLFWwindow *window, int width,
                                     int height) {
  Window *_this = static_cast<Window *>(glfwGetWindowUserPointer(window));

  if (_this) {
    _this->HandleFrameBufferSize(width, height);
  }
}

void Window::HandleFrameBufferSize(int width, int height) {
  m_width = width;
  m_height = height;

  // Reset the opengl viewport with the new dimension
  glViewport(0, 0, width, height);

  for (auto listener : m_fbsListeners)
    listener->OnFrameBufferSize(width, height);
}

void Window::HandleGLFWError(int error, const char *description) {
  std::cout << "[Error]: GLFW (" << error << "): " << description << std::endl;
}

void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  Window *_this = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (_this) {
    _this->HandleKey(key, scancode, action, mods);
  }
}

void Window::HandleKey(int key, int scancode, int action, int mods) {
  if (m_prevKeyCallback != nullptr)
    m_prevKeyCallback(m_window, key, scancode, action, mods);

  m_input.HandleKey(key, action, mods);
}

void Window::CursorPosCallback(GLFWwindow *window, double x, double y) {
  Window *_this = static_cast<Window *>(glfwGetWindowUserPointer(window));
  if (_this) {
    _this->HandleCursorPos(x, y);
  }
}

void Window::HandleCursorPos(double x, double y) {
  m_input.HandleCursorPosition(x, y);
}
