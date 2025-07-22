#include "Renderer.hpp"

#include <memory>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>

namespace OGL {

/////// RENDERER ///////
std::shared_ptr<Shader> Renderer::defaultMeshShader;

Renderer::Renderer(Camera &&camera, int width, int height) {
  if (!defaultMeshShader) {
    defaultMeshShader = std::make_shared<Shader>("res/shaders/mesh.vert",
                                                 "res/shaders/mesh.frag");
  }

  m_width = width;
  m_height = height;
  m_camera = std::move(camera);

  // Framebuffer object
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  // Color attachment
  m_colorTexture = std::make_unique<OGL_Texture2D>();
  ResizeColorBuffer(width, height);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                       m_colorTexture->m_texId, 0);

  // Depth attachment
  glGenRenderbuffers(1, &m_depthBuffer);
  ResizeDepthBuffer(width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, m_depthBuffer);

  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, drawBuffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::runtime_error("Framebuffer error");
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer() { glDeleteFramebuffers(1, &m_fbo); }

void Renderer::SetupObjects(
    const std::vector<std::shared_ptr<I_Object3D>> &objects) {

  for (auto obj : objects) {
    if (std::shared_ptr<ArrayMesh> ptr =
            std::dynamic_pointer_cast<ArrayMesh>(obj)) {
      obj->Finalize();

      // TODO: Check if the object is a light source
      m_objs.push_back(ptr);
    }
  }
}

std::shared_ptr<I_Texture2D> Renderer::Render() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
  glViewport(0, 0, m_width, m_height);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto obj : m_objs) {
    auto shader = obj->GetShader();
    shader->Use();
    shader->setUniformMat4f("uModel", obj->m_T);
    shader->setUniformMat4f("uView", m_camera.m_view->GetViewMatrix());
    shader->setUniformMat4f("uProjection",
                            m_camera.m_proj->GetProjectionMatrix());

    glBindVertexArray(obj->m_vao);
    glDrawElements(GL_TRIANGLES, obj->m_numIndices, GL_UNSIGNED_INT, 0);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return m_colorTexture;
}

void Renderer::OnFrameBufferSize(int width, int height) {
  m_width = width;
  m_height = height;

  ResizeColorBuffer(width, height);
  ResizeDepthBuffer(width, height);
}

void Renderer::ResizeColorBuffer(int width, int height) {
  glBindTexture(GL_TEXTURE_2D, m_colorTexture->m_texId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
void Renderer::ResizeDepthBuffer(int width, int height) {
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/////// ArrayMesh ///////

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

  m_shader = Renderer::defaultMeshShader;
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

std::shared_ptr<ArrayMesh> LoadModel(const std::string &filepath) {
  std::shared_ptr<ArrayMesh> mesh = std::make_shared<ArrayMesh>();

  std::vector<glm::vec3> pos;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> norm;
  std::vector<uint32_t> indices;

  uint32_t index_offset = 0;

  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenNormals |
                                      aiProcess_CalcTangentSpace |
                                      aiProcess_JoinIdenticalVertices);

  if (nullptr == scene || !scene->HasMeshes()) {
    throw std::runtime_error(importer.GetErrorString());
  }

  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    const aiMesh *ai_mesh = scene->mMeshes[i];

    for (unsigned int j = 0; j < ai_mesh->mNumVertices; j++) {
      pos.emplace_back(ai_mesh->mVertices[j].x, ai_mesh->mVertices[j].y,
                       ai_mesh->mVertices[j].z);
      norm.emplace_back(ai_mesh->mNormals[j].x, ai_mesh->mNormals[j].y,
                        ai_mesh->mNormals[j].z);
      if (ai_mesh->HasTextureCoords(0)) {
        uvs.emplace_back(ai_mesh->mTextureCoords[0][j].x,
                         ai_mesh->mTextureCoords[0][j].y);
      } else {
        uvs.emplace_back(0.0f, 0.0f);
      }
    }

    for (unsigned int j = 0; j < ai_mesh->mNumFaces; j++) {
      const aiFace &face = ai_mesh->mFaces[j];
      for (unsigned int k = 0; k < face.mNumIndices; k++) {
        indices.push_back(face.mIndices[k] + index_offset);
      }
    }
  }

  mesh->SubData(pos, uvs, norm, indices);

  std::cout << "Imported mesh: " << scene->mName.C_Str() << " with "
            << scene->mNumMeshes << " sub meshes, " << pos.size()
            << " vertices and " << indices.size() << " indices" << std::endl;

  return mesh;
}

} // namespace OGL
