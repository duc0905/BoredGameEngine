#pragma once
#include <glm/glm.hpp>
#include <cstring>
#include <memory>
#include <assimp/scene.h>
#include "../Adapter/Render.h"
#include "../Adapter/OGL.h"

namespace Bored
{
namespace Render
{

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

/**
 * TODO: Move somewhere
 */
struct OGLMesh : public IMesh
{
  public:
    OGLMesh()
    {
        m_vao.Bind();
        m_ebo.Bind();

        m_posVbo.SetLayout({{"Pos", Float3}});
        m_uvsVbo.SetLayout({{"UVs", Float2}});
        m_normVbo.SetLayout({{"Norm", Float3}});

        // Attach the VBOs to VAO
        m_vao.AttachBuffer(m_posVbo);
        m_vao.AttachBuffer(m_uvsVbo);
        m_vao.AttachBuffer(m_normVbo);

        m_ebo.Unbind();
        m_vao.Unbind();
    }

    OGLMesh(IMesh& other) : OGLMesh()
    {
        m_vao.Bind();

        subPos(other.getPos());
        subUVs(other.getUVs());
        subNorms(other.getNorms());

        subIndices(other.getIndices());

        m_vao.Unbind();
    }

    virtual std::vector<glm::vec3> getPos() const override
    {
        std::vector<char> data = m_posVbo.GetData();
        std::vector<glm::vec3> pos(data.begin(), data.end());
        return pos;
    }
    virtual std::vector<glm::vec2> getUVs() const override
    {
        std::vector<char> data = m_posVbo.GetData();
        std::vector<glm::vec2> uvs(data.begin(), data.end());
        return uvs;
    }
    virtual std::vector<glm::vec3> getNorms() const override
    {
        std::vector<char> data = m_posVbo.GetData();
        std::vector<glm::vec3> norm(data.begin(), data.end());
        return norm;
    }
    virtual std::vector<unsigned int> getIndices() const override
    {
        std::vector<char> data = m_posVbo.GetData();
        std::vector<unsigned int> indices(data.begin(), data.end());
        return indices;
    }

    virtual void subPos(std::vector<glm::vec3> p_pos) override
    {
        unsigned int const size = p_pos.size() * sizeof(glm::vec3);
        char* s = (char*)malloc(size + 1);
        memcpy(s, &p_pos, size);
        std::string s2(s);
        std::vector<char> pos(s2.begin(), s2.end());
        m_posVbo.SubData(pos);
    }

    virtual void subUVs(std::vector<glm::vec2> p_uvs) override
    {
        unsigned int const size = p_uvs.size() * sizeof(glm::vec2);
        char* s = (char*)malloc(size + 1);
        memcpy(s, &p_uvs, size);
        std::string s2(s);
        free(s);
        std::vector<char> uvs(s2.begin(), s2.end());
        m_posVbo.SubData(uvs);
    }

    virtual void subNorms(std::vector<glm::vec3> p_norms) override
    {
        unsigned int const size = p_norms.size() * sizeof(glm::vec3);
        char* s = (char*)malloc(size + 1);
        memcpy(s, &p_norms, size);
        std::string s2(s);
        free(s);
        std::vector<char> norms(s2.begin(), s2.end());
        m_posVbo.SubData(norms);
    }

    virtual void subIndices(std::vector<unsigned int> p_indices) override
    {
        m_ebo.SubData(p_indices);
    }

  private:
    // TODO: Think about how to attach the vbos
    //
    Bored::Render::OGL::VertexArray m_vao;
    Bored::Render::OGL::VertexBuffer m_posVbo, m_uvsVbo, m_normVbo;
    Bored::Render::OGL::IndexBuffer m_ebo;
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
