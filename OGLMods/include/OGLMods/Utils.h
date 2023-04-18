#pragma once
#include "../../oglpch.h"
#include "ShaderClass.h"

namespace Bored::OGL
{
  class Renderer;

struct Texture : public Bored::Render::Texture
{
  unsigned int id;
  bool isInit = false;

  Texture() = default;
  /**
   * @brief Create OGL texture based on Bored::Render::Texture
   * @param tex Base texture that holds data of the texture.
  */
  Texture(std::shared_ptr<Bored::Render::Texture> tex);
  void Bind(Shader& shader, const std::string& uniName, unsigned int texSlot = 0) const;
  void Delete();
};

struct Material : public Bored::Render::Material
{
    bool isInit = false;

    // Overshadowing
    std::shared_ptr<Bored::OGL::Texture> diffuse, specular;

    Material() = default;
    Material(std::shared_ptr<Bored::Render::Material> mat);
};

struct Model : public Bored::Render::Model
{
  bool isInit = false;

  // Overshadowing
  std::vector<std::pair <
    std::shared_ptr<Bored::Render::Mesh>,
    std::shared_ptr<Bored::OGL::Material>>> renderables;

  Model() = default;
  Model(Bored::Render::Model& mod);
};
}
