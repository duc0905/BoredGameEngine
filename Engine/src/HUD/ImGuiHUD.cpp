#include "pch.h"
#include "ImGuiHUD.h"
#include "../IGame.h"
#include "../Window/GLFWWindow.h"

ImGuiHUD::ImGuiHUD()
{
	std::shared_ptr<IWindow> iwindow = IGame::GetWindowPtr();
	std::shared_ptr<GLFWWindow> window = std::dynamic_pointer_cast<GLFWWindow>(iwindow);
	if (!window)
	{
		LOG_COLOR("You are trying to use ImGuiHUD system. It is currently only support GLFW and OpenGL3.", COLOR::RED, COLOR::BLACK);
		LOG_COLOR("You must use GLFWWindow in order to use the ImGuiHUD system.", COLOR::BRIGHT_BLUE, COLOR::BLACK);
		return;
	}

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

ImGuiHUD::~ImGuiHUD()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiHUD::OnTick(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::Begin("Hello World!");
	//ImGui::Text("abcd", "1234");
	for (auto& window : windows_)
		window->Render();

	//ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

IHUDWindow& ImGuiHUD::AddWindow()
{
	windows_.push_back(std::make_unique<ImGuiHUDWindow>());
	return *windows_.back();
}
