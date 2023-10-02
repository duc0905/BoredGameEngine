#pragma once
#include "FileExplorer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <memory>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#include "FileContent.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

class Editor {
    private:
        Editor() {}
    public:
        ~Editor() {}

        static Editor& GetInstance() {
            if (!instance)
                instance = std::unique_ptr<Editor>(new Editor());
            return *instance;
        }

        int Init() {
            glfwSetErrorCallback(glfw_error_callback);
            if (!glfwInit()) return 1;

            // Decide GL+GLSL versions
            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 330";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            // Create window with graphics context
            window = glfwCreateWindow(
                    1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
            if (window == nullptr) return 1;
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);  // Enable vsync

            int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            if (version == 0) {
                printf("Failed to initialize OpenGL context\n");
                return -1;
            }

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            (void)io;
            io.ConfigFlags |=
                ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
            io.ConfigFlags |=
                ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            //   ImGuiWindowFlags flags |= ImGuiWindowFlags_

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);

            clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            fex = std::make_shared<Bored::Editor::FileExplorer>(90, 720);
            contentWindow = std::make_shared<Bored::Editor::FileContentWindow>(720, 720, nullptr);

            // cast from BoredWindow to FileExplorer and call set callback
            fex->SetOpenFileCallBack([&](std::shared_ptr<Bored::FileSystem::File> file) {
                    contentWindow->SetFileToDisplay(file);
                    contentWindow->SetOpen(true);
                    });

            return 0;

        }

        void Run() {
            // Main loop
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();

                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

                if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

                fex->Create();
                contentWindow->Create();

                // Rendering
                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(window, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                        clear_color.z * clear_color.w, clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(window);
            }
#ifdef __EMSCRIPTEN__
            EMSCRIPTEN_MAINLOOP_END;
#endif
        }

        void Shutdown() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(window);
            glfwTerminate();
        }

    private:
        static std::unique_ptr<Editor> instance;

        std::shared_ptr<Bored::Editor::FileExplorer> fex = nullptr;
        std::shared_ptr<Bored::Editor::FileContentWindow> contentWindow = nullptr;

        GLFWwindow* window = nullptr;

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color;
};

std::unique_ptr<Editor> Editor::instance = nullptr;


// Main code
int main(int, char**) {

    Editor& editor = Editor::GetInstance();

    if (editor.Init() != 0) {
        return -1;
    }
    editor.Run();
    editor.Shutdown();

    return 0;
}
