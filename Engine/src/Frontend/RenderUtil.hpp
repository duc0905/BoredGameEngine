#pragma once
#include <glm/glm.hpp>
#include <cstring>
#include <memory>
#include <assimp/scene.h>
// #include "../Adapter/OGL.h"

namespace Bored
{
namespace Render
{

class ITexture
{
  public:
    virtual void* GetId() const = 0;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void SubData(unsigned width, unsigned height, unsigned int bpp, void* data) = 0;

  public:
    std::string _name;
    unsigned int _width;
    unsigned int _height;
    unsigned int _bpp;
};

class CPUTexture : public ITexture
{
public:
    ~CPUTexture()
    {
        if (m_hasData)
        {
            free(m_data);
        }
    }
  public:
    virtual void* GetId() const
    {
        return nullptr;
    }

    virtual void Bind() const
    {
    }

    virtual void Unbind() const
    {
    }

    virtual void SubData(unsigned p_width, unsigned p_height, unsigned int p_bpp, void* p_data)
    {
        size_t size = p_width * p_height * p_bpp;
        m_data = (char*)malloc(size);
        memcpy(m_data, p_data, size);
        m_width = p_width;
        m_height = p_height;
        m_bpp = p_bpp;
        m_hasData = true;
    }

  private:
    void* m_data;
    bool m_hasData = false;
    unsigned int m_width = 0, m_height = 0, m_bpp = 4;
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
    virtual std::vector<glm::vec3> getPos() const = 0;
    virtual std::vector<glm::vec2> getUVs() const = 0;
    virtual std::vector<glm::vec3> getNorms() const = 0;
    virtual std::vector<unsigned int> getIndices() const = 0;
    virtual void subPos(std::vector<glm::vec3> pos) = 0;
    virtual void subUVs(std::vector<glm::vec2> uvs) = 0;
    virtual void subNorms(std::vector<glm::vec3> norms) = 0;
    virtual void subIndices(std::vector<unsigned int> indices) = 0;
};

/**
 * Represent the vertices, indices, and UV coordinates for the Model
 */
struct CPUMesh : public IMesh
{
  public:
    virtual std::vector<glm::vec3> getPos() const override
    {
        return m_pos;
    }
    virtual std::vector<glm::vec2> getUVs() const override
    {
        return m_uvs;
    }
    virtual std::vector<glm::vec3> getNorms() const override
    {
        return m_norms;
    }
    virtual std::vector<unsigned int> getIndices() const override
    {
        return m_indices;
    }

    virtual void subPos(std::vector<glm::vec3> pos) override
    {
        this->m_pos = pos;
    }
    virtual void subUVs(std::vector<glm::vec2> uvs) override
    {
        this->m_uvs = uvs;
    }
    virtual void subNorms(std::vector<glm::vec3> norms) override
    {
        this->m_norms = norms;
    }
    virtual void subIndices(std::vector<unsigned int> indices) override
    {
        this->m_indices = indices;
    }

  private:
    std::vector<glm::vec3> m_pos;
    std::vector<glm::vec2> m_uvs;
    std::vector<glm::vec3> m_norms;
    std::vector<unsigned int> m_indices;
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

void GetMaterials(const aiScene* scene, std::vector<std::shared_ptr<Material>>& mats);

void GetTextures(const aiScene* scene, std::vector<std::shared_ptr<ITexture>>& texs
                 //, Render::Context* renderContext
);

void GetMeshes(const aiScene* scene, std::vector<std::shared_ptr<IMesh>>& meshes);

void ProcessScene(const aiScene* scene, std::vector<std::shared_ptr<Material>>& mats,
                  std::vector<std::shared_ptr<ITexture>>& texs, std::vector<std::shared_ptr<IMesh>>& meshes);

std::shared_ptr<Model> LoadModel(const std::string& file);

} // namespace Render
} // namespace Bored
