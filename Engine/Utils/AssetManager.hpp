#pragma once

#include "../Components/Mesh3D.hpp"
#include "../Components/MeshComponent.hpp"
#include <memory>
#include <string>
#include <unordered_map>

#include "../Systems/Renderer/I_Texture.hpp"

namespace Bored {

class AssetManager {
public:
  ~AssetManager() {};
  static AssetManager &GetInstance() {
    if (!instance) {
      instance = std::unique_ptr<AssetManager>(new AssetManager());
      instance->m_models =
          std::unordered_map<std::string, std::shared_ptr<MeshComponent>>();
      instance->m_textures =
          std::unordered_map<std::string, std::shared_ptr<I_Texture2D>>();
      instance->m_materials =
          std::unordered_map<std::string, std::shared_ptr<Material>>();
    }
    return *instance;
  }

  std::shared_ptr<MeshComponent> LoadModel(const std::string &filepath);
  std::shared_ptr<I_Texture2D> LoadTexture(const std::string &filepath);

private:
  static std::unique_ptr<AssetManager> instance;
  std::unordered_map<std::string, std::shared_ptr<MeshComponent>> m_models;
  std::unordered_map<std::string, std::shared_ptr<I_Texture2D>> m_textures;
  std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
};
} // namespace Bored