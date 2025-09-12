#pragma once

#include "Components/Lighting.hpp"
#include "Components/TransformComponent.hpp"
#include "Scene/Node.hpp"
#include <entt/entity/storage.hpp>
#include <entt/entt.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include <imgui.h>
#include <memory>

template<>
struct entt::type_name<glm::vec3> {
    static constexpr std::string_view value() noexcept {
        return "Vec3";
    }
};

template<>
struct entt::type_name<glm::vec2> {
    static constexpr std::string_view value() noexcept {
        return "Vec2";
    }
};

template<>
struct entt::type_name<glm::mat4> {
    static constexpr std::string_view value() noexcept {
        return "Mat4";
    }
};

template<>
struct entt::type_name<glm::mat3> {
    static constexpr std::string_view value() noexcept {
        return "Mat3";
    }
};

template<>
struct entt::type_name<glm::mat2> {
    static constexpr std::string_view value() noexcept {
        return "Mat2";
    }
};

class NodeWindow {
public:
  NodeWindow() { 
    // TODO: More types
    // TODO: What about user-defined types
    Bored::TransformComponent::Register();
    Bored::PointLight::Register();
    Bored::DirectionalLight::Register();
  }

  void OnSelectNode(std::shared_ptr<Bored::Node> p_node) { node = p_node; }

  void Render() {
    ImGui::Begin("Node");
    if (!node) {
      ImGui::Text("No node selected");
    } else {
      ImGui::Text("%s", node->name.c_str());

      // Iterate through every registered components
      for (auto &&[id, type] : entt::resolve()) {
        auto *storage = node->registry.storage(type.id());

        // Check if the entity has this component
        if (storage && storage->contains(node->id)) {
          auto *raw = storage->value(node->id);
          auto any = type.from_void(raw);

          ImGui::Text("%s", type.info().name().data());

          // Iterate through each field
          for (auto &&[data_id, data] : type.data()) {
            auto data_type = data.type();
            std::string field_name = data.custom();

            if (data_type == entt::resolve<glm::vec3>()) {
              auto value = data.get(any).cast<glm::vec3>();
              if (ImGui::InputFloat3(field_name.c_str(), &value.x)) {
                data.set(any, value);
              }
            }

            // TODO: Render other field types - int, float, double, string, etc.
          }
        }
      }
    }

    ImGui::End();
  }

private:
  std::shared_ptr<Bored::Node> node;
};
