#include "CPU_Renderer2D.hpp"
#include "../../Components/FlatComponent.hpp"
#include "../../Scene/Scene.hpp"
#include "../Renderer/Texture/CPU_Texture.hpp"
#include <memory>

namespace Bored {

CPU_Renderer2D::CPU_Renderer2D(int width, int height) {
  pixels = std::make_shared<CPU_Texture2D>();
  pixels->WriteData(std::vector<std::byte>(width * height * 3), {width, height},
                    3);
}

void CPU_Renderer2D::OnUpdate(double dt, Scene &scene) {
  auto texture = Render(scene);
  scene.context.io->Render(texture);
}

std::shared_ptr<I_Texture2D> CPU_Renderer2D::Render(Scene &scene) {
  // TODO: implement
  //
  // Get all FlatComponents
  //
  // For each component
  //    Check if it is of type container
  //
  //    Check if it is of type text
  //
  //    Check if it is of type button

  auto view = scene.ecs_registry.view<FlatComponent>();

  for (auto &&[id, flat_comp] : view->each()) {
    flat_comp.element->Render(pixels);
  }

  return pixels;
}

}; // namespace Bored
