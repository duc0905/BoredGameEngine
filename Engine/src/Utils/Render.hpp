#pragma once
#include "../Adapter/Render.h"
#include <glm/glm.hpp>

namespace Bored
{
namespace Render
{
// struct Texture
// {
//     std::string name;
//     unsigned int width, height, bpp;
//     unsigned char* data;
// };

struct Material
{
    std::string name;
    float opacity = 1.0f;
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
};

struct Light
{
    glm::vec3 color;
    float ambient, diffuse, specular;
};

struct Mesh
{
    std::string name;
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> indices;
};

typedef std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>> Renderable;
struct Model
{
    std::vector<Renderable> renderables;

    Model() : renderables({})
    {
    }
    Model(const Model& other) : renderables(other.renderables)
    {
    }
};

class TextureFactory
{
  private:
    static std::shared_ptr<Texture> Find(const std::string& path);
    TextureFactory() = delete;
    ~TextureFactory() = delete;

  public:
    static std::vector<std::shared_ptr<Texture>> storage;
    static std::shared_ptr<Texture> Load(const std::string& path);
    static std::shared_ptr<Texture> Load(const std::string& name, unsigned int w, unsigned int h, unsigned int bpp,
                                         unsigned char* data);
    static std::shared_ptr<Texture> Load(const Texture& tex);
};

class MaterialFactory
{
    static std::vector<std::shared_ptr<Material>> storage;
    static std::shared_ptr<Material> Find(const std::string& path);
    // MaterialFactory() = delete;
  public:
    static std::shared_ptr<Material> Load(const std::string& name, float shininess, float reflectiveness,
                                          std::shared_ptr<Texture> diff = nullptr,
                                          std::shared_ptr<Texture> spec = nullptr);
    static std::shared_ptr<Material> Load(const Material& mat);
    static std::shared_ptr<Material> Load(const std::string& name);
    MaterialFactory() = delete;
    ~MaterialFactory() = delete;
};

class MeshFactory
{
    static std::vector<std::shared_ptr<Mesh>> storage;
    static std::shared_ptr<Mesh> Find(const std::string& name);
    MeshFactory() = delete;

  public:
    static std::shared_ptr<Mesh> Load(const std::string& name);
    static std::shared_ptr<Mesh> Load(const std::string& name, std::vector<glm::vec3>, std::vector<glm::vec2>,
                                      std::vector<glm::vec3>, std::vector<unsigned int>);
    static std::shared_ptr<Mesh> Load(const Mesh& mesh);
    // MeshFactory() = delete;
    ~MeshFactory() = delete;
};

} // namespace Render

namespace Helper
{
Render::Model Load(const std::string& file);
}
} // namespace Bored
