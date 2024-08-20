#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <filesystem>

#include <Engine/Adapter/OGL.h>

#include "Editor.hpp"
#include "GameManifest/Loader.hpp"
#include "SubWindow/FileExplorer.hpp"
#include "SubWindow/FileContent.hpp"
#include "SubWindow/GameScreen.hpp"

std::unique_ptr<Editor> Editor::_instance = nullptr;

Editor& Editor::GetInstance() {
    if (!_instance)
        _instance = std::unique_ptr<Editor>(new Editor());
    return *_instance;
}

void Editor::Init() {
    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    mainWindow = Bored::GLFW::Window::GetInstance();

    mainWindow->SetTitle("Bored Editor");
    mainWindow->SetWidth(1280);
    mainWindow->SetHeight(720);
    mainWindow->OnSetup();
    mainWindow->UseRenderContext(Bored::Render::OGL::Context::GetDefault());

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
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)mainWindow->GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    auto fex = std::make_shared<FileExplorer>(_game, 90, 720);
    auto contentWindow = std::make_shared<FileContentWindow>(_game, 720, 720, nullptr);
    auto gameScreen = std::make_shared<GameScreen>(_game);
    contentWindow->SetOpen(false);
    fex->SetOpenFileCallBack([=](std::shared_ptr<Bored::FileSystem::File> file) {
        contentWindow->SetFileToDisplay(file);
        contentWindow->SetOpen(true);
    });

    fex->SetOpenProjectCallback([this](std::string dir) {
        OpenProject(dir);
    });

    windows.push_back(fex);
    windows.push_back(contentWindow);
    windows.push_back(gameScreen);

    for (auto w : windows)
    w->OnSetup();
}

void Editor::Run() {
    bool isRunning = true;
    while (isRunning)
    {
        // Logic updates
        isRunning &= mainWindow->OnUpdate(0.016f);
        for (auto w : windows) isRunning &= w->OnUpdate(0.016f);

        // Draw contents
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        for (auto w : windows) w->Create();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // mainWindow->NewFrame();
    }
}

void Editor::Shutdown() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (auto w : windows)
    w->OnShutdown();

    mainWindow->OnShutdown();
}

void Editor::OpenProject(const std::string& directory) {
    // TODO: Implement this method

    if (!std::filesystem::exists(directory)) {
        std::cerr << "Directory " << directory << "DNE" << std::endl;
        return;
    }

    std::filesystem::path man_file = std::filesystem::path(directory) / "Bored.json";
    if (!std::filesystem::exists(man_file)) {
        std::cerr << "Manifest file DNE in " << directory << std::endl;
        return;
    }

    std::cout << "Yay :)" << std::endl;
    _game = LoadManifestFile(man_file.string());
    _game.path = directory;
    PrintManifest(_game);
}
