#include "OGLMesh.hpp"
#include <iostream>
#include "../Adapter/Render.h"

namespace Bored
{
namespace Render
{
namespace OGL
{

Mesh::Mesh()
{
    m_vao.Bind();
    m_ebo.Bind();

    m_posVbo.SetLayout({{"Pos", Render::Float3}});
    m_uvsVbo.SetLayout({{"UVs", Render::Float2}});
    m_normVbo.SetLayout({{"Norm", Render::Float3}});

    // Attach the VBOs to VAO
    m_vao.AttachBuffer(m_posVbo);
    m_vao.AttachBuffer(m_uvsVbo);
    m_vao.AttachBuffer(m_normVbo);

    m_ebo.Unbind();
    m_vao.Unbind();
}

Mesh::Mesh(IMesh& other) : Mesh()
{
    m_vao.Bind();

    subPos(other.getPos());
    subUVs(other.getUVs());
    subNorms(other.getNorms());

    subIndices(other.getIndices());

    m_vao.Unbind();
}

std::vector<glm::vec3> Mesh::getPos() const
{
    std::vector<char> data = m_posVbo.GetData();
    std::vector<glm::vec3> pos(data.size() / sizeof(glm::vec3));
    memcpy(pos.data(), data.data(), data.size());
    return pos;
}
std::vector<glm::vec2> Mesh::getUVs() const
{
    std::vector<char> data = m_uvsVbo.GetData();
    std::vector<glm::vec2> uvs(data.size() / sizeof(glm::vec2));
    memcpy(uvs.data(), data.data(), data.size());
    return uvs;
}
std::vector<glm::vec3> Mesh::getNorms() const
{
    std::vector<char> data = m_normVbo.GetData();
    std::vector<glm::vec3> norm(data.size() / sizeof(glm::vec3));
    memcpy(norm.data(), data.data(), data.size());
    return norm;
}
std::vector<unsigned int> Mesh::getIndices() const
{
    std::vector<char> data = m_posVbo.GetData();
    std::vector<unsigned int> indices(data.begin(), data.end());
    return indices;
}

void Mesh::subPos(std::vector<glm::vec3> p_pos)
{
    size_t const size = p_pos.size() * sizeof(glm::vec3);
    // std::byte* s = (std::byte*)malloc(size);
    // memcpy(s, &p_pos, size);
    // std::vector<char> pos(s, s + size);
    // m_posVbo.SubData(s, size);
    // free(s);
    m_posVbo.SubData(p_pos.data(), size);
}

void Mesh::subUVs(std::vector<glm::vec2> p_uvs)
{
    size_t const size = p_uvs.size() * sizeof(glm::vec2);
    // std::byte* s = (std::byte*)malloc(size);
    // memcpy(s, &p_uvs, size);
    // std::vector<char> uvs(s, s + size);
    // m_posVbo.SubData(s, size);
    // free(s);
    m_uvsVbo.SubData(p_uvs.data(), size);
}

void Mesh::subNorms(std::vector<glm::vec3> p_norms)
{
    size_t const size = p_norms.size() * sizeof(glm::vec3);
    // std::byte* s = (std::byte*)malloc(size);
    // memcpy(s, &p_norms, size);
    // std::vector<char> norms(s, s + size);
    // m_posVbo.SubData(s, size);
    // free(s);
    m_normVbo.SubData(p_norms.data(), size);
}

void Mesh::subIndices(std::vector<unsigned int> p_indices)
{
    m_ebo.SubData(p_indices);
}

} // namespace OGL
} // namespace Render
} // namespace Bored
