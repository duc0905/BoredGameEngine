#pragma once
#include <glm/glm.hpp>
#include <memory>
#include "../Adapter/Render.h"

namespace Bored
{
namespace Render
{
struct Material
{
    std::string name;
    float opacity = 1.0f;
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
};

// TODO:
// struct Light
// {
//     glm::vec3 color;
//     float ambient, diffuse, specular;
// };

/**
 * Represent the vertices, indices, and UV coordinates for the Model
 */
struct Mesh
{
    std::string name;
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> norms;
    std::vector<unsigned int> indices;
};

typedef std::pair<std::shared_ptr<Mesh>, std::shared_ptr<Material>> Renderable;

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
