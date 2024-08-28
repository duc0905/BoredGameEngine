#pragma once

#include <glm/glm.hpp>
#include "../RenderUtil.hpp"
#include "../../Adapter/OGL.h"

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
    virtual Bored::Render::VertexArray* GetVertexArray() override { return &m_vao; }
    virtual Bored::Render::IndexBuffer* GetIndexBuffer() override { return &m_ebo; }

    virtual std::vector<glm::vec3> GetPos() const override;
    virtual std::vector<glm::vec2> GetUVs() const override;
    virtual std::vector<glm::vec3> GetNorms() const override;
    virtual std::vector<unsigned int> GetIndices() const override;

    virtual void SubPos(std::vector<glm::vec3> p_pos) override;
    virtual void SubUVs(std::vector<glm::vec2> p_uvs) override;
    virtual void SubNorms(std::vector<glm::vec3> p_norms) override;
    virtual void SubIndices(std::vector<unsigned int> p_indices) override;

  private:
    Bored::Render::OGL::VertexArray m_vao;
    Bored::Render::OGL::VertexBuffer m_posVbo, m_uvsVbo, m_normVbo;
    Bored::Render::OGL::IndexBuffer m_ebo;
};

} // namespace OGL
} // namespace Render
} // namespace Bored
