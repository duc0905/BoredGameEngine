#include "AssetManager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace Bored {

// Define the static member
std::unique_ptr<AssetManager> AssetManager::instance = nullptr;

std::shared_ptr<MeshComponent>
AssetManager::LoadModel(const std::string &filepath) {

  std::shared_ptr<MeshComponent> mesh_component =
      std::make_shared<MeshComponent>();

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

  // get material
  unsigned int num_materials = scene->mNumMaterials;
  if (num_materials > 0) {
    const aiMaterial *material = scene->mMaterials[0];

    aiColor3D diffuse(0.8f, 0.8f, 0.8f);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    aiColor3D specular(0.2f, 0.2f, 0.2f);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    aiColor3D ambient(0.1f, 0.1f, 0.1f);
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    float shininess = 2.0f;
    material->Get(AI_MATKEY_SHININESS, shininess);

    mesh_component->material = std::make_shared<Material>(
        glm::vec3{ambient.r, ambient.g, ambient.b},
        glm::vec3{diffuse.r, diffuse.g, diffuse.b},
        glm::vec3{specular.r, specular.g, specular.b}, shininess);

  } else {
    mesh_component->material = std::make_shared<Material>(
        glm::vec3{0.1f, 0.1f, 0.1f}, glm::vec3{0.8f, 0.8f, 0.8f},
        glm::vec3{0.2f, 0.2f, 0.2f}, 2.0f);
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

  mesh_component->mesh = std::make_shared<ArrayMesh>();
  mesh_component->mesh->SubData(pos, uvs, norm, indices);

  std::cout << "Imported mesh: " << scene->mName.C_Str() << " with "
            << scene->mNumMeshes << " sub meshes, " << pos.size()
            << " vertices and " << indices.size() << " indices" << std::endl;

  std::cout << "Material: \n \tAmbient: " << mesh_component->material->ambient.x
            << ", " << mesh_component->material->ambient.y << ", "
            << mesh_component->material->ambient.z << std::endl;
  std::cout << "\tDiffuse: " << mesh_component->material->diffuse.x << ", "
            << mesh_component->material->diffuse.y << ", "
            << mesh_component->material->diffuse.z << std::endl;
  std::cout << "\tSpecular: " << mesh_component->material->specular.x << ", "
            << mesh_component->material->specular.y << ", "
            << mesh_component->material->specular.z << std::endl;
  std::cout << "\tShininess: " << mesh_component->material->shininess
            << std::endl;
  return mesh_component;
}

std::shared_ptr<I_Texture2D>
AssetManager::LoadTexture(const std::string &filepath) {
  return nullptr;
}
} // namespace Bored