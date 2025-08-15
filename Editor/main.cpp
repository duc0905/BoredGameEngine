#include "MyScene.hpp"
#include "Scene/Scene.hpp"
#include "Services/IOService.hpp"
#include "Windows/NodeWindow.hpp"
#include "Windows/SceneTree.hpp"
#include <format>
#include <functional>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

void DockSpace(SceneTree &scene, NodeWindow &node_window) {
  static bool first_time = true;

  // Dockspace
  ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  // window_flags |= ImGuiWindowFlags_NoBackground;

  bool ds_open = true;
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace playground", &ds_open, window_flags);
  ImGui::PopStyleVar();
  ImGui::PopStyleVar(2);
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  ImGuiIO &io = ImGui::GetIO();

  if (ImGui::BeginMenuBar()) { // Tool bar
    if (ImGui::BeginMenu("Options")) {
      // Disabling fullscreen would allow the window to be moved to the front
      // of other windows, which we can't undo at the moment without finer
      // window depth/z control.
      // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
      // ImGui::MenuItem("Padding", NULL, &opt_padding);
      // ImGui::Separator();

      if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "",
                          (dockspace_flags &
                           ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode;
      }
      if (ImGui::MenuItem(
              "Flag: NoDockingSplit", "",
              (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit;
      }
      if (ImGui::MenuItem("Flag: NoUndocking", "",
                          (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) !=
                              0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking;
      }
      if (ImGui::MenuItem("Flag: NoResize", "",
                          (dockspace_flags & ImGuiDockNodeFlags_NoResize) !=
                              0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
      }
      if (ImGui::MenuItem(
              "Flag: AutoHideTabBar", "",
              (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) {
        dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
      }

      ImGui::Separator();

      if (ImGui::MenuItem("Close", NULL, false, true))
        ds_open = false;
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  // Split horizontally
  if (first_time) {
    first_time = false;
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id,
                              dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    auto dock_id_asset = ImGui::DockBuilderSplitNode(
        dockspace_id, ImGuiDir_Down, 0.3f, nullptr, &dockspace_id);

    auto dock_id_fs = ImGui::DockBuilderSplitNode(
        dock_id_asset, ImGuiDir_Left, 0.3f, nullptr, &dock_id_asset);

    auto dock_id_main = ImGui::DockBuilderSplitNode(
        dockspace_id, ImGuiDir_Left, 0.7f, nullptr, &dockspace_id);

    auto dock_id_tree = ImGui::DockBuilderSplitNode(
        dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);

    ImGui::DockBuilderDockWindow("Asset", dock_id_asset);
    ImGui::DockBuilderDockWindow("Filesystem", dock_id_fs);
    ImGui::DockBuilderDockWindow("Main", dock_id_main);
    ImGui::DockBuilderDockWindow("Scene", dock_id_tree);
    ImGui::DockBuilderDockWindow("Node", dockspace_id);
  }

  ImGui::End();

  ImGui::Begin("Asset");
  ImGui::Text("Asset manager");
  ImGui::End();

  ImGui::Begin("Filesystem");
  ImGui::Text("File system");
  ImGui::End();

  ImGui::Begin("Main");
  ImGui::Text("Main window");
  ImGui::End();

  // ImGui::Begin("Scene");
  // ImGui::Text("Scene tree");
  // ImGui::End();
  scene.Render();

  node_window.Render();

  // ImGui::Begin("Component");
  // ImGui::Text("Components");
  // ImGui::End();
}

int main() {
  MyScene scene;
  scene.BuildScene();

  Bored::IOService io_service(800, 600, true);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;           // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport /
                                                      // Platform Windows
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // When viewports are enabled we tweak WindowRounding/WindowBg so platform
  // windows can look identical to regular ones.
  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(io_service.window, true);
  ImGui_ImplOpenGL3_Init();

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.35f, 0.10f, 1.00f);

  SceneTree scene_window(scene);
  NodeWindow node_window;
  scene_window.on_select_node =
      std::bind(&NodeWindow::OnSelectNode, &node_window, std::placeholders::_1);

  while (!glfwWindowShouldClose(io_service.window)) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags.
    glfwPollEvents();
    if (glfwGetWindowAttrib(io_service.window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DockSpace(scene_window, node_window);

    // 1. Show the big demo window (Most of the sample code is in
    // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
    // ImGui!).
    if (show_demo_window)
      ImGui::ShowDemoWindow(&show_demo_window);

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(io_service.window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we
    // save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call
    //  glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(io_service.window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
