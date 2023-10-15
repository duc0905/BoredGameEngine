#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
 #include <glad/glad.h>
// #include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "Bored.hpp"

#include "window.h"
#include "GameScreen.hpp"
#include "FileContent.h"
#include "FileExplorer.h"

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

class Editor
{
  private:
    Editor()
    {
    }

  public:
    ~Editor()
    {
    }

    static Editor& GetInstance()
    {
        if (!instance)
            instance = std::unique_ptr<Editor>(new Editor());
        return *instance;
    }

    int Init()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;

        // Decide GL+GLSL versions
        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        mainWindow.SetTitle("Bored Editor");
        mainWindow.SetWidth(1280);
        mainWindow.SetHeight(720);
        mainWindow.OnSetup();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //   ImGuiWindowFlags flags |= ImGuiWindowFlags_

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(mainWindow.GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        auto fex = std::make_shared<Bored::Editor::FileExplorer>(90, 720);
        auto contentWindow = std::make_shared<Bored::Editor::FileContentWindow>(720, 720, nullptr);
        auto gameScreen = std::make_shared<Bored::Editor::GameScreen>();

        fex->SetOpenFileCallBack([&](std::shared_ptr<Bored::FileSystem::File> file) {
            contentWindow->SetFileToDisplay(file);
            contentWindow->SetOpen(true);
        });


        windows.push_back(fex);
        windows.push_back(contentWindow);
        windows.push_back(gameScreen);

        return 0;
    }

    void Run()
    {
        bool isRunning = true;
        // Main loop
        while (isRunning)
        {
            isRunning &= mainWindow.OnUpdate(0.016f);
            mainWindow.ClearColor();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            for (auto w : windows)
                w->Create();

            int display_w = mainWindow.GetWidth();
            int display_h = mainWindow.GetHeight();

            //glViewport(0, 0, display_w, display_h);
            //glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
            //             clear_color.w);
            //glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
#ifdef __EMSCRIPTEN__
        EMSCRIPTEN_MAINLOOP_END;
#endif
    }

    void Shutdown()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        for (auto w : windows)
            w->Shutdown();

        mainWindow.OnShutdown();
    }

  private:
    static std::unique_ptr<Editor> instance;

    std::vector<std::shared_ptr<Bored::Editor::Window>> windows;

    Bored::GLFW::Window mainWindow;

    //GLFWwindow* window = nullptr;

    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color;
};

std::unique_ptr<Editor> Editor::instance = nullptr;

// Main code
int main(int, char**)
{
    Editor& editor = Editor::GetInstance();

    if (editor.Init() != 0)
    {
        return -1;
    }
    editor.Run();
    editor.Shutdown();

    return 0;
}
