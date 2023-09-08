#pragma once
#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include "../Adapter/Render.h"

namespace Bored {
  namespace Window {

    class Window {
    public:
      Window();
      ~Window();

      void SetWidth(unsigned int);
      void SetHeight(unsigned int);
      void SetTitle(const std::string&);
      void SetFullscreen(bool);

      unsigned int GetWidth() const;
      unsigned int GetHeight() const;

      bool OnTick(double dt);

      void SwapBuffer() {
        glfwSwapBuffers(nativeWindow);
      }

      void PollEvents() {
        glfwPollEvents();
      }

      bool ShouldClose() {
        return glfwWindowShouldClose(nativeWindow);
      }

      template <class T,
        std::enable_if_t<std::is_base_of_v<Bored::Render::Context, T>, bool> = true>
      void Start() {
        int nMonitors = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&nMonitors);

        GLFWmonitor* monitor = NULL;
        if (fullscreen)
          monitor = monitors[0];

        nativeWindow = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);

        if (!nativeWindow) {
          glfwTerminate();
          throw std::exception("Cannot create GLFW window");
        }

        glfwMakeContextCurrent(nativeWindow);
        glfwSwapInterval(1);

        renderContext = std::make_unique<T>(this);
      }
    private:
      std::string name;
      unsigned int width = 800, height = 600;
      bool fullscreen = false;
      std::unique_ptr<Bored::Render::Context> renderContext;

      GLFWwindow* nativeWindow = nullptr;
    };
  }
}