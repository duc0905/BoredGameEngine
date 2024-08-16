#pragma once

#include <glm/glm.hpp>
#include "RenderUtil.hpp"
#include "../Adapter/OGL.h"

namespace Bored
{
namespace Render
{
namespace OGL
{

/**
 * TODO: Move somewhere
 */
struct Mesh : public IMesh
{
  friend class ShaderProgram;
  public:
    Mesh();
    Mesh(IMesh& other);

  public:
    virtual std::vector<glm::vec3> getPos() const override;
    virtual std::vector<glm::vec2> getUVs() const override;
    virtual std::vector<glm::vec3> getNorms() const override;
    virtual std::vector<unsigned int> getIndices() const override;

    virtual void subPos(std::vector<glm::vec3> p_pos) override;
    virtual void subUVs(std::vector<glm::vec2> p_uvs) override;
    virtual void subNorms(std::vector<glm::vec3> p_norms) override;
    virtual void subIndices(std::vector<unsigned int> p_indices) override;

  private:
    Bored::Render::OGL::VertexArray m_vao;
    Bored::Render::OGL::VertexBuffer m_posVbo, m_uvsVbo, m_normVbo;
    Bored::Render::OGL::IndexBuffer m_ebo;
};

} // namespace OGL
} // namespace Render
} // namespace Bored
