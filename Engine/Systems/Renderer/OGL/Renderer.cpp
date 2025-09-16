#include "Renderer.hpp"
#include "../../../Components/Camera.hpp"
#include "../../../Components/Lighting.hpp"
#include "../../../Components/MeshComponent.hpp"
#include "../../../Components/TransformComponent.hpp"

#include <format>
#include <glm/matrix.hpp>
#include <memory>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>

namespace OGL {

/////// RENDERER ///////
std::shared_ptr<Shader> Renderer::defaultMeshShader;
std::shared_ptr<Bored::Material> Renderer::defaultMaterial =
    std::make_shared<Bored::Material>(glm::vec3{0.8f, 0.8f, 0.8f}, 0.1f, 0.7f, 0.2f, 2.0f);

Renderer::Renderer(Bored::IOService &io_service) : io(io_service) {
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

  auto fb_size = io_service.GetFrameBufferSize();
  m_width = fb_size.first;
  m_height = fb_size.second;

  // Framebuffer object
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  // Color attachment
  m_colorTexture = std::make_unique<OGL_Texture2D>();
  ResizeColorBuffer(m_width, m_height);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                       m_colorTexture->m_texId, 0);

  // Depth attachment
  glGenRenderbuffers(1, &m_depthBuffer);
  ResizeDepthBuffer(m_width, m_height);
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

  io_service.SetFrameBufferSizeHandler(std::bind(&Renderer::OnFrameBufferSize,
                                                 this, std::placeholders::_1,
                                                 std::placeholders::_2));
}

Renderer::~Renderer() { glDeleteFramebuffers(1, &m_fbo); }

void Renderer::SetupObjects(Bored::Scene &scene) {
  auto mesh_view = scene.ecs_registry.view<Bored::MeshComponent>();

  for (auto &&[entity, mesh_comp] : mesh_view.each()) {
    if (mesh_comp.mesh) {
      if (!mesh_comp.mesh->GetShader()) {
        mesh_comp.mesh->SetShader(defaultMeshShader);
      }

      if (!mesh_comp.material) {
        mesh_comp.material = defaultMaterial;
      }
    }
  }
}

void Renderer::OnFrameBufferSize(int width, int height) {
  m_width = width;
  m_height = height;

  ResizeColorBuffer(width, height);
  ResizeDepthBuffer(width, height);
}

std::shared_ptr<I_Texture2D> Renderer::Render(Bored::Scene &scene) {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glViewport(0, 0, m_width, m_height);
  glClearColor(m_bg.r, m_bg.g, m_bg.b, m_bg.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera
  glm::mat4 view_mat(1.0f);
  glm::mat4 proj_mat(1.0f);
  glm::vec3 cam_eye(0.0f);
  {
    auto cam_node = scene.GetActiveCamera();
    if (!cam_node) {
      throw std::runtime_error("Active scene has no active camera");
    }

    if (!scene.ecs_registry.all_of<Bored::TransformComponent,
                                   Bored::CameraComponent>(cam_node->id)) {
      throw std::runtime_error(
          "Camera Node does not have Camera component or transform component");
    }

    Bored::CameraComponent &cam_comp =
        scene.ecs_registry.get<Bored::CameraComponent>(cam_node->id);

    glm::mat4 cam_model_mat = cam_node->GetGlobalTransformMatrix();

    view_mat = glm::inverse(cam_model_mat);
    proj_mat = cam_comp.m_proj->GetProjectionMatrix();
    cam_eye = cam_model_mat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  }

  // Get all lights
  auto point_light_view =
      scene.ecs_registry.view<Bored::TransformComponent, Bored::PointLight>();
  auto dir_light_view =
      scene.ecs_registry
          .view<Bored::TransformComponent, Bored::DirectionalLight>();

  // Get all meshes
  auto mesh_view =
      scene.ecs_registry.view<Bored::NodeComponent, Bored::MeshComponent>();

  for (auto &&[entity, node_comp, mesh_comp] : mesh_view.each()) {
    // Getting hierarchical transformation matrix
    glm::mat4 model_mat = node_comp.node->GetGlobalTransformMatrix();

    auto mesh = mesh_comp.mesh;
    auto material = mesh_comp.material;
    if (!mesh)
      continue;

    auto shader = mesh->GetShader();
    shader->Use();

    // MVP
    shader->setUniformMat4f("uModel", model_mat);
    // TODO: Consider using uniform buffer for these 3 stuff
    shader->setUniformMat4f("uView", view_mat);
    shader->setUniformMat4f("uProjection", proj_mat);
    shader->setUniformVec3f("uEye", cam_eye);

    // NOTE: Material
    shader->setUniformVec3f("uMaterial.color", material->color);
    shader->setUniformFloat("uMaterial.ambient", material->ambient);
    shader->setUniformFloat("uMaterial.diffuse", material->diffuse);
    shader->setUniformFloat("uMaterial.specular", material->specular);
    shader->setUniformFloat("uMaterial.shininess", material->shininess);

    // NOTE: Light
    int pls_index = 0;
    int dls_index = 0;

    // Point light sources
    for (auto &&[entity, light_trans, light_comp] : point_light_view.each()) {
      shader->setUniformVec3f(std::format("pointLights[{}].color", pls_index),
                              light_comp.light_color);
      shader->setUniformVec3f(
          std::format("pointLights[{}].position", pls_index),
          light_trans.translate);
      shader->setUniformFloat(
          std::format("pointLights[{}].strength", pls_index),
          light_comp.strength);

      pls_index++;
    }

    // NOTE: Directional lights
    for (auto &&[entity, light_trans, light_comp] : dir_light_view.each()) {
      shader->setUniformVec3f(std::format("dirLights[{}].color", dls_index),
                              light_comp.light_color);
      shader->setUniformVec3f(std::format("dirLights[{}].direction", dls_index),
                              light_comp.GetDirection(light_trans));
      dls_index++;
    }

    // NOTE: Render calls
    glBindVertexArray(mesh->m_vao);
    glDrawElements(GL_TRIANGLES, mesh->m_numIndices, GL_UNSIGNED_INT, 0);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return m_colorTexture;
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
