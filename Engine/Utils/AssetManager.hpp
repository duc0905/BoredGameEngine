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
  AssetManager() = default;
  ~AssetManager() = default;

  std::shared_ptr<MeshComponent> LoadModel(const std::string &filepath);
  std::shared_ptr<I_Texture2D> LoadTexture(const std::string &filepath);

private:
  std::unordered_map<std::string, std::shared_ptr<ArrayMesh>> m_models;
  std::unordered_map<std::string, std::shared_ptr<I_Texture2D>> m_textures;
  std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
};
} // namespace Bored