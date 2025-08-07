#include "Mesh3D.hpp"
#include <glad/glad.h>

namespace Bored {
ArrayMesh::ArrayMesh() {
  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(3, m_vbos);

  // Position buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(0);

  // UV buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(1);

  // Normal buffer
  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glEnableVertexAttribArray(2);

  // EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  glBindVertexArray(0);
}

ArrayMesh::~ArrayMesh() {
  glDeleteBuffers(3, m_vbos);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

std::vector<glm::vec3> ArrayMesh::GetPositions() const {
  std::vector<glm::vec3> ret(m_numVertices);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_numVertices * sizeof(glm::vec3),
                     ret.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return ret;
}

std::vector<glm::vec2> ArrayMesh::GetUVs() const {
  std::vector<glm::vec2> ret(m_numVertices);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_numVertices * sizeof(glm::vec2),
                     ret.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return ret;
}
std::vector<glm::vec3> ArrayMesh::GetNormals() const {
  std::vector<glm::vec3> ret(m_numVertices);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_numVertices * sizeof(glm::vec3),
                     ret.data());
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return ret;
}
std::vector<uint32_t> ArrayMesh::GetIndices() const {
  std::vector<uint32_t> ret(m_numIndices);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                     m_numIndices * sizeof(uint32_t), ret.data());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return ret;
}

void ArrayMesh::SubData(const std::vector<glm::vec3> &p_pos,
                        const std::vector<glm::vec2> &p_uvs,
                        const std::vector<glm::vec3> &p_norms,
                        const std::vector<uint32_t> &p_indices) {
  // Update positions
  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, p_pos.size() * sizeof(glm::vec3), p_pos.data(),
               GL_STATIC_DRAW);

  // Update texcoords
  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
  glBufferData(GL_ARRAY_BUFFER, p_uvs.size() * sizeof(glm::vec2), p_uvs.data(),
               GL_STATIC_DRAW);

  // Update normals
  glBindBuffer(GL_ARRAY_BUFFER, m_vbos[2]);
  glBufferData(GL_ARRAY_BUFFER, p_norms.size() * sizeof(glm::vec3),
               p_norms.data(), GL_STATIC_DRAW);

  // Update indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_indices.size() * sizeof(uint32_t),
               p_indices.data(), GL_STATIC_DRAW);

  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  m_numVertices = p_pos.size();
  m_numIndices = p_indices.size();
}
} // namespace Bored
