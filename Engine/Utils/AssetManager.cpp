#include "AssetManager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cstring>
#include <assimp/types.h>
#include <filesystem>
#include <format>
#include <fstream>
#include <glm/glm.hpp>
#include <ios>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../Systems/Renderer/Texture/OGL_Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Bored {

// Define the static member
std::unique_ptr<AssetManager> AssetManager::instance = nullptr;

std::shared_ptr<MeshComponent>
AssetManager::LoadModel(const std::string &filepath) {
  // Check if the model is already loaded
  if (m_models.find(filepath) != m_models.end()) {
    return m_models[filepath];
  }

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
    std::cout << "Number of materials: " << num_materials << std::endl;
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

    aiString str;
    if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &str)) {
      std::filesystem::path path =
          std::filesystem::path(filepath).parent_path();
      std::filesystem::path diffuse_path = path / str.C_Str();
      mesh_component->material->diffuse_texture =
          LoadTexture(diffuse_path.string());
    }
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

  if (mesh_component->material->diffuse_texture) {
    std::cout << "\tDiffuse texture: "
              << mesh_component->material->diffuse_texture->GetSize().x << ", "
              << mesh_component->material->diffuse_texture->GetSize().y
              << std::endl;
  }

  m_models[filepath] = mesh_component;
  return mesh_component;
}

std::shared_ptr<I_Texture2D>
AssetManager::LoadTexture(const std::string &filepath) {
  if (m_textures.find(filepath) != m_textures.end()) {
    return m_textures[filepath];
  }

  std::shared_ptr<I_Texture2D> texture = std::make_shared<OGL_Texture2D>();

  stbi_set_flip_vertically_on_load(true);
  int width, height, bpp;
  unsigned char *data = stbi_load(filepath.c_str(), &width, &height, &bpp, 0);

  if (!data) {
    throw std::runtime_error("Failed to load texture: " + filepath);
  }

  std::vector<std::byte> data_vec;
  data_vec.reserve(width * height * bpp);
  for (int i = 0; i < width * height * bpp; ++i) {
    data_vec.push_back(static_cast<std::byte>(data[i]));
  }

  texture->WriteData(data_vec, glm::vec2{width, height}, bpp);
  stbi_image_free(data);

  m_textures[filepath] = texture;

  return texture;
}

/* Fonts */

struct FontDirectory {};

struct OffsetSubtable {
  uint32_t scaler_type;   /*< Indicate the OFA scaler to be used. */
  uint16_t numTables;     /*< Number of tables */
  uint16_t searchRange;   /*< (Maximum power of 2 <= numTables) * 16 */
  uint16_t entrySelector; /*< log_2(maximum power of 2 <= numTables) */
  uint16_t rangeShift;    /*< numTables * 16 - searchRange */

  friend std::ostream &operator<<(std::ostream &os,
                                  const OffsetSubtable &subtable) {
    os << "\tScaler type: " << subtable.scaler_type << std::endl;
    os << "\tNumber of tables: " << subtable.numTables << std::endl;
    os << "\tSearch range: " << subtable.searchRange << std::endl;
    os << "\tEntry selector: " << subtable.entrySelector << std::endl;
    os << "\tRange shift: " << subtable.rangeShift << std::endl;

    return os;
  }
};

struct TableDirectoy {
  uint32_t tag;      /*< Table name */
  uint32_t checkSum; /*< Checksum of the table content */
  uint32_t offset;   /*< Offset from beginning of sfnt */
  uint32_t length;   /*< Length of the table in byte (not including padding) */

  friend std::ostream &operator<<(std::ostream &os,
                                  const TableDirectoy &directory) {
    char name[5];
    memcpy(name, &directory.tag, 4);

    os << "\t\tTable name: " << name << std::endl;
    os << "\t\tCheck sum: " << directory.checkSum << std::endl;
    os << "\t\tOffset: " << directory.offset << std::endl;
    os << "\t\tLength: " << directory.length << std::endl;

    return os;
  }
};

/**
 * Converting from little endian to big endian and vice-versa.
 *
 * Reversing the bytes of the variable.
 */
template <typename T>
void ReverseBytes(T& value) {
  uint8_t numBytes = sizeof(T);
  char* temp = new char[numBytes];
  memcpy(temp, &value, numBytes);

  for (int i = 0; i < numBytes / 2; i++) {
    temp[i] += temp[numBytes - 1 - i];
    temp[numBytes - 1 - i] = temp[i] - temp[numBytes - 1 - i];
    temp[i] = temp[i] - temp[numBytes - 1 - i];
  }

  value = *(T*)temp;
  delete[] temp;
}

std::shared_ptr<Font> AssetManager::LoadFontTTF(const std::string &filepath) {
  // TODO: implement
  std::ifstream file(filepath, std::ios::binary);
  std::filesystem::path path(filepath);

  if (!file) {
    throw std::runtime_error(std::format("Error while opening file {}", filepath));
  }

  std::cout << "Font: " << path.filename() << std::endl;

  OffsetSubtable offset_subtable;
  std::vector<TableDirectoy> table_dirs;

  // Big-endian to little endian
  file.read((char*)&offset_subtable.scaler_type, sizeof(uint32_t));
  ReverseBytes(offset_subtable.scaler_type);

  file.read((char*)&offset_subtable.numTables, sizeof(uint16_t));
  ReverseBytes(offset_subtable.numTables);

  file.read((char*)&offset_subtable.searchRange, sizeof(uint16_t));
  ReverseBytes(offset_subtable.searchRange);

  file.read((char*)&offset_subtable.entrySelector, sizeof(uint16_t));
  ReverseBytes(offset_subtable.entrySelector);

  file.read((char*)&offset_subtable.rangeShift, sizeof(uint16_t));
  ReverseBytes(offset_subtable.rangeShift);

  std::cout << offset_subtable << std::endl;

  table_dirs.resize(offset_subtable.numTables);

  for (int i = 0; i < offset_subtable.numTables; i++) {
    // Read table entries
    file.read((char*)&table_dirs[i].tag, sizeof(uint32_t));
    file.read((char*)&table_dirs[i].checkSum, sizeof(uint32_t));
    ReverseBytes(table_dirs[i].checkSum);
    file.read((char*)&table_dirs[i].offset, sizeof(uint32_t));
    ReverseBytes(table_dirs[i].offset);
    file.read((char*)&table_dirs[i].length, sizeof(uint32_t));
    ReverseBytes(table_dirs[i].length);

    std::cout << "=================" << i << "=================" << std::endl;
    std::cout << table_dirs[i] << std::endl;
  }

  return nullptr;
}
} // namespace Bored
