#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "../Adapter/Render.h"
#include <iostream>
#include <assimp/scene.h>

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
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> indices;

    virtual std::vector<glm::vec3> getPos() const override
    {
        return pos;
    }
    virtual std::vector<glm::vec2> getUVs() const override
    {
        return uvs;
    }
    virtual std::vector<glm::vec3> getNorms() const override
    {
        return norms;
    }
    virtual std::vector<unsigned int> getIndices() const override
    {
        return indices;
    }

    virtual void subPos(std::vector<glm::vec3> pos) override
    {
        this->pos = pos;
    }
    virtual void subUVs(std::vector<glm::vec2> uvs) override
    {
        this->uvs = uvs;
    }
    virtual void subNorms(std::vector<glm::vec3> norms) override
    {
        this->norms = norms;
    }
    virtual void subIndices(std::vector<unsigned int> indices) override
    {
        this->indices = indices;
    }
};

struct OGLMesh : public IMesh
{
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> indices;

    virtual std::vector<glm::vec3> getPos() const override
    {
        return pos;
    }
    virtual std::vector<glm::vec2> getUVs() const override
    {
        return uvs;
    }
    virtual std::vector<glm::vec3> getNorms() const override
    {
        return norms;
    }
    virtual std::vector<unsigned int> getIndices() const override
    {
        return indices;
    }
    virtual void subPos(std::vector<glm::vec3> pos) override
    {
        this->pos = pos;
    }
    virtual void subUVs(std::vector<glm::vec2> uvs) override
    {
        this->uvs = uvs;
    }
    virtual void subNorms(std::vector<glm::vec3> norms) override
    {
        this->norms = norms;
    }
    virtual void subIndices(std::vector<unsigned int> indices) override
    {
        this->indices = indices;
    }
    // unsigned int vao, vbo, ebo;
    // unsigned int numIndices;

    // virtual std::vector<glm::vec3> getPos() const override
    // {

    // }
    // virtual std::vector<glm::vec2> getUVs() const override
    // {
    //     return {};
    // }
    // virtual std::vector<glm::vec3> getNorms() const override
    // {
    //     return {};
    // }
    // virtual std::vector<unsigned int> getIndices() const override
    // {
    //     return {};
    // }
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

// TODO: Description
class Projector
{
  public:
    // Get the projection matrix for this projector
    [[nodiscard]] virtual glm::mat4 GetMat() const = 0;
};

// TODO: Description
class Camera
{
  public:
    glm::vec3 dir = {1.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    float yaw = 0.0f, pitch = 0.0f;

    // Get the direction the camera is facing
    [[nodiscard]] glm::vec3 GetDir() const;
    // Get the view matrix for this camera
    [[nodiscard]] glm::mat4 GetViewMat(const glm::vec3& pos) const;
};

// TODO: Description
class OrthoProjector : public Projector
{
    float l, r, t, b;

  public:
    OrthoProjector(float left, float right, float bottom, float top) : l(left), r(right), t(top), b(bottom)
    {
    }

    [[nodiscard]] virtual glm::mat4 GetMat() const override;
};

// TODO: Description
class PerspProjector : public Projector
{
  public:
    const unsigned int &w, h;
    float fov = 30.0f;
    float zNear = 0.1f, zFar = 100.0f;

  public:
    PerspProjector(const unsigned int& width, const unsigned int& height) : w(width), h(height)
    {
    }
    [[nodiscard]] virtual glm::mat4 GetMat() const override;
};
} // namespace Render
} // namespace Bored
