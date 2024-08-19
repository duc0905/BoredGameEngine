#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <assimp/scene.h>
#include "../Adapter/Render.h"

namespace Bored
{
namespace Render
{

class ITexture
{
  public:
    virtual Render::NativeTexture& GetNativeTexture() = 0;

  public:
    std::string m_name;
};


struct Material
{
    std::string name;
    float opacity = 1.0f;
    std::shared_ptr<ITexture> diffuse;
    std::shared_ptr<ITexture> specular;
};

// TODO:
// struct Light
// {
//     glm::vec3 color;
//     float ambient, diffuse, specular;
// };

struct IMesh
{
    std::string name;
    virtual ~IMesh() = default;

    virtual Bored::Render::VertexArray* GetVertexArray() = 0;
    virtual Bored::Render::IndexBuffer* GetIndexBuffer() = 0;

    virtual std::vector<glm::vec3> GetPos() const = 0;
    virtual std::vector<glm::vec2> GetUVs() const = 0;
    virtual std::vector<glm::vec3> GetNorms() const = 0;
    virtual std::vector<unsigned int> GetIndices() const = 0;

    virtual void SubPos(std::vector<glm::vec3> pos) = 0;
    virtual void SubUVs(std::vector<glm::vec2> uvs) = 0;
    virtual void SubNorms(std::vector<glm::vec3> norms) = 0;
    virtual void SubIndices(std::vector<unsigned int> indices) = 0;
};

typedef std::pair<std::shared_ptr<IMesh>, std::shared_ptr<Material>> Renderable;

/**
 * Represent a 3D model
 */
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

class IFactory
{
  public:
    virtual std::shared_ptr<IMesh> CreateMesh() = 0;
    virtual std::shared_ptr<IMesh> CreateMesh(IMesh& other) = 0;
    virtual std::shared_ptr<Material> CreateMaterial() = 0;
    virtual std::shared_ptr<Material> CreateMaterial(Material& other) = 0;
    virtual std::shared_ptr<ITexture> CreateTexture() = 0;
    virtual std::shared_ptr<ITexture> CreateTexture(ITexture& other) = 0;

  public:
    virtual ~IFactory() = default;
};

std::shared_ptr<Model> LoadModel(const std::string& file);

} // namespace Render
} // namespace Bored
