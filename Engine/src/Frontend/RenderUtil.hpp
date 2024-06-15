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

    Model() : renderables({}) {}
    Model(const Model& other) : renderables(other.renderables) {}
};
}
}
