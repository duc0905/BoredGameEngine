#pragma once

#include <vector>
#include <memory>
#include "../Systems/Renderer/Shader/Shader.hpp"

namespace Bored {
struct Material {
  float ambient;
  float diffuse;
  float specular;

  float shininess;
};

struct ArrayMesh {
public:
  ArrayMesh();
  virtual ~ArrayMesh();

  std::vector<glm::vec3> GetPositions() const;
  std::vector<glm::vec2> GetUVs() const;
  std::vector<glm::vec3> GetNormals() const;
  std::vector<uint32_t> GetIndices() const;

  void SubData(const std::vector<glm::vec3> &p_pos,
               const std::vector<glm::vec2> &p_uvs,
               const std::vector<glm::vec3> &p_norms,
               const std::vector<uint32_t> &m_indices);

  std::shared_ptr<Shader> GetShader() const { return m_shader; }
  void SetShader(std::shared_ptr<Shader> shader) { m_shader = shader; }

protected:
  std::shared_ptr<Shader> m_shader;

public:
  // 0: position, 1: uvs, 2: normals
  GLuint m_vao, m_vbos[3], m_ebo;
  unsigned int m_numVertices, m_numIndices;

  std::shared_ptr<Material> material;
};
} // namespace Bored
