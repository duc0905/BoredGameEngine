#include "Renderer.hpp"

#include <exception>
#include <format>
#include <memory>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <typeinfo>

namespace OGL {

/////// RENDERER ///////
std::shared_ptr<Shader> Renderer::defaultMeshShader;
std::shared_ptr<Bored::Material> Renderer::defaultMaterial =
    std::make_shared<Bored::Material>(0.1f, 0.7f, 0.2f, 2.0f);

Renderer::Renderer(int width, int height) {
  if (!defaultMeshShader) {
    defaultMeshShader = std::make_shared<Shader>("res/shaders/mesh.vert",
                                                 "res/shaders/mesh.frag");
    defaultMeshShader->Use();
    for (int i = 0; i < 6; i++) {
      defaultMeshShader->setUniformVec3f(
          std::format("pointLights[{}].color", i), {0.0f, 0.0f, 0.0f});
      defaultMeshShader->setUniformVec3f(
          std::format("pointLights[{}].position", i), {0.0f, 0.0f, 0.0f});
      defaultMeshShader->setUniformFloat(
          std::format("pointLights[{}].strength", i), 0.0f);

      defaultMeshShader->setUniformVec3f(std::format("dirLights[{}].color", i),
                                         {0.0f, 0.0f, 0.0f});
      defaultMeshShader->setUniformVec3f(
          std::format("dirLights[{}].direction", i), {0.0f, 0.0f, 0.0f});
    }
  }

  m_width = width;
  m_height = height;

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

void Renderer::SetupObjects(std::shared_ptr<Bored::Scene> scene) {
  scene->Traverse([this](std::shared_ptr<Bored::Node> node) {
    // NOTE: Only care about ArrayMesh for now
    if (std::shared_ptr<Bored::ArrayMesh> mesh =
            std::dynamic_pointer_cast<Bored::ArrayMesh>(node)) {
      if (!mesh->GetShader()) {
        mesh->SetShader(defaultMeshShader);
      }

      if (!mesh->material) {
        mesh->material = defaultMaterial;
      }
    }

    if (std::shared_ptr<Bored::Light> light =
            std::dynamic_pointer_cast<Bored::Light>(node)) {
      m_lights.push_back(light);
    }
  });
}

std::shared_ptr<I_Texture2D> Renderer::Render() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glViewport(0, 0, m_width, m_height);
  glClearColor(m_bg.r, m_bg.g, m_bg.b, m_bg.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  active_scene->Traverse([this](std::shared_ptr<Bored::Node> node) {
    // If node is a mesh
    if (std::shared_ptr<Bored::ArrayMesh> mesh =
            dynamic_pointer_cast<Bored::ArrayMesh>(node)) {

      auto shader = mesh->GetShader();
      shader->Use();
      shader->setUniformVec3f(
          "uEye", active_scene->GetActiveCamera()->transform.translate);

      // MVP
      shader->setUniformMat4f("uModel", mesh->GetModelMatrix());
      shader->setUniformMat4f("uView",
                              active_scene->GetActiveCamera()->GetViewMatrix());
      shader->setUniformMat4f(
          "uProjection",
          active_scene->GetActiveCamera()->m_proj->GetProjectionMatrix());

      // Material
      shader->setUniformFloat("uMaterial.ambient", mesh->material->ambient);
      shader->setUniformFloat("uMaterial.diffuse", mesh->material->diffuse);
      shader->setUniformFloat("uMaterial.specular", mesh->material->specular);
      shader->setUniformFloat("uMaterial.shininess", mesh->material->shininess);

      // Lights
      int pointLightIdx = 0;
      int dirLightIdx = 0;
      for (auto light : m_lights) {
        if (std::shared_ptr<Bored::PointLight> l =
                std::dynamic_pointer_cast<Bored::PointLight>(light)) {
          shader->setUniformVec3f(
              std::format("pointLights[{}].color", pointLightIdx),
              l->light_color);
          shader->setUniformVec3f(
              std::format("pointLights[{}].position", pointLightIdx),
              l->transform.translate);
          shader->setUniformFloat(
              std::format("pointLights[{}].strength", pointLightIdx),
              l->strength);
          pointLightIdx++;
        }

        if (std::shared_ptr<Bored::DirectionalLight> l =
                std::dynamic_pointer_cast<Bored::DirectionalLight>(light)) {
          shader->setUniformVec3f(
              std::format("dirLights[{}].color", dirLightIdx), l->light_color);
          shader->setUniformVec3f(
              std::format("dirLights[{}].direction", dirLightIdx), l->GetDirection());
          dirLightIdx++;
        }
      }

      glBindVertexArray(mesh->m_vao);
      glDrawElements(GL_TRIANGLES, mesh->m_numIndices, GL_UNSIGNED_INT, 0);
    }
  });

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

std::shared_ptr<Bored::ArrayMesh> LoadModel(const std::string &filepath) {
  std::shared_ptr<Bored::ArrayMesh> mesh = std::make_shared<Bored::ArrayMesh>();

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
