#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
// #include <iostream>
#include <memory>

// #include "Bored.hpp"

#include "SubWindow.h"
#include "GameScreen.hpp"
#include "FileContent.h"
#include "FileExplorer.h"

Bored::Game game;

class Editor
{
  private:
    static std::unique_ptr<Editor> instance;

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
        // Decide GL+GLSL versions
        // GL 3.0 + GLSL 130
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        mainWindow.SetTitle("Bored Editor");
        mainWindow.SetWidth(1280);
        mainWindow.SetHeight(720);
        mainWindow.OnSetup();
        mainWindow.UseRenderContext(Bored::Render::OGL::Context::GetDefault());

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        //   ImGuiWindowFlags flags |= ImGuiWindowFlags_

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        const char* glsl_version = "#version 330";
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)mainWindow.GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        auto fex = std::make_shared<Bored::Editor::FileExplorer>(90, 720);
        auto contentWindow = std::make_shared<Bored::Editor::FileContentWindow>(720, 720, nullptr);
        auto gameScreen = std::make_shared<Bored::Editor::GameScreen>();
        contentWindow->SetOpen(true);
        fex->SetOpenFileCallBack([=](std::shared_ptr<Bored::FileSystem::File> file) {
            contentWindow->SetFileToDisplay(file);
            contentWindow->SetOpen(true);
        });

        windows.push_back(fex);
        windows.push_back(contentWindow);
        windows.push_back(gameScreen);

        for (auto w : windows)
            w->OnSetup();

        return 0;
    }

    void Run()
    {
        // TODO: Use GameLoop instead
        bool isRunning = true;
        while (true)
        {
            // Logic updates
            isRunning &= mainWindow.OnUpdate(0.016f);
            for (auto w : windows)
                w->OnUpdate(0.016f);

            // Draw contents
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            for (auto w : windows)
                w->Create();

            mainWindow.GetRenderer().BindFramebuffer();
            // glBindFramebuffer(GL_FRAMEBUFFER, 0);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            mainWindow.DrawContent();
        }
    }

    void Shutdown()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        for (auto w : windows)
            w->OnShutdown();

        mainWindow.OnShutdown();
    }

  private:
    Bored::GLFW::Window mainWindow;
    // Sub windows
    std::vector<std::shared_ptr<Bored::Editor::SubWindow>> windows;

    bool show_demo_window = false;
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
