#pragma once

#include "Scene/Node.hpp"
#include "Scene/Scene.hpp"
#include <imgui.h>

class SceneTree {
public:
  SceneTree(Bored::Scene &scene) : scene(scene) {}

  void Render() {
    ImGui::Begin("Scene", &open);

    ImGui::Text("Scene: %s", scene.name.c_str());
    int i = 0;
    RenderNode(scene.GetRoot(), i);
    // scene.Traverse([this, &i](std::shared_ptr<Bored::Node> node) {
    //   RenderNode(node, i);
    //   i++;
    // });

    ImGui::End();
  }

public:
  std::function<void(std::shared_ptr<Bored::Node>)> on_select_node;

private:
  void RenderNode(std::shared_ptr<Bored::Node> node, int &i) {
    if (!node)
      return;

    ImGuiTreeNodeFlags node_flags = base_node_flags;
    const bool is_selected = ((selection_mask & (1 << i)) != 0);

    if (is_selected) {
      node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool node_open = ImGui::TreeNodeEx((void *)(intptr_t)i, node_flags, "%s",
                                       node->name.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
      selection_mask = (1 << i);
      if (on_select_node)
        on_select_node(node);
    }

    i++;
    for (auto child : node->children) {
      RenderNode(child, i);
    }

    if (node_open)
      ImGui::TreePop();
  }

private:
  Bored::Scene &scene;
  bool open = true;

private:
  int selection_mask = 0;
  ImGuiTreeNodeFlags base_node_flags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
      ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
};
