// #include "pch.h"
// #include "../Core.h"
// #include "Utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>

#include "Render.hpp"

using namespace Bored::Render;
using namespace std;

std::vector<std::shared_ptr<Texture>>TextureFactory::storage;
std::vector<std::shared_ptr<Material>>MaterialFactory::storage;
std::vector<std::shared_ptr<Mesh>>MeshFactory::storage;

/* TODO: Fix this file */

shared_ptr<Texture> TextureFactory::Load(const string& path) {
  shared_ptr<Texture> tex = Find(path);
  if (tex == nullptr) {
    int w, h, bpp;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.c_str(), &w, &h, &bpp, 4);

    if (data) {
      tex = make_shared<Texture>(
        path, (unsigned int)w, (unsigned int)h, (unsigned int)bpp, data);
    } else {
      // Bored::logger->warn("Failed to load file: " + path);
      unsigned char white[4] = { 0xff, 0xff, 0xff, 0xff };
      tex = Load(path, 1, 1, 4, white);
      // logger->warn("Using white texture for: " + path);
    }

    storage.push_back(tex);
    return tex;
  }
  return tex;
}

std::shared_ptr<Texture> TextureFactory::Load(const std::string& name,
  unsigned int w, unsigned int h, unsigned int bpp,
  unsigned char* data) {
  shared_ptr<Texture> t = Find(name);
  if (t == nullptr) {
    if (bpp > 4 || bpp <= 0)
    {
      // logger->warn("BPP cannot be other than 1, 2, 3, 4. Assuming BPP = 4.");
      bpp = 4;
    }

    unsigned char* newData = new unsigned char[w * h * 4];
    memcpy(newData, data, sizeof(unsigned char) * w * h * 4);
    t = make_shared<Texture>(
      name, w, h, bpp, newData);
    storage.push_back(t);
  }
  return t;
}

std::shared_ptr<Texture>
Bored::Render::TextureFactory::Load(const Texture& tex) {
  // shared_ptr<Texture> t = Find(tex.name);
  if (t == nullptr) {
    t = make_shared<Texture>(tex);
    storage.push_back(t);
  }
  return t;
}

shared_ptr<Texture> TextureFactory::Find(const std::string& path) {
  for (auto text : storage)
    if (text->name == path)
      return text;
  return nullptr;
}

std::shared_ptr<Mesh> MeshFactory::Find(const std::string& path) {
  for (auto mesh : storage)
    if (mesh->name == path)
      return mesh;
  return nullptr;
}

std::shared_ptr<Mesh> MeshFactory::Load(const std::string& name) {
  std::shared_ptr<Mesh> m = Find(name);
  if (!m) {
    // TODO change this to a cube
    m = make_shared<Mesh>(name);
    storage.push_back(m);
  }
  return m;
}

std::shared_ptr<Mesh> MeshFactory::Load(const std::string& name,
  std::vector<glm::vec3> pos,
  std::vector<glm::vec2> uvs,
  std::vector<glm::vec3> norms,
  std::vector<unsigned int> indices) {
  std::shared_ptr<Mesh> m = Find(name);
  if (!m) {
    m = make_shared<Mesh>(name, pos, uvs, norms, indices);
    storage.push_back(m);
  }
  return m;
}

std::shared_ptr<Mesh> MeshFactory::Load(const Mesh& mesh) {
  std::shared_ptr<Mesh> m = Find(mesh.name);
  if (!m) {
    m = make_shared<Mesh>(mesh);
    storage.push_back(m);
  }
  return m;
}

std::shared_ptr<Material> MaterialFactory::Find(const std::string& path) {
  for (auto mat : storage)
    if (mat->name == path)
      return mat;
  return nullptr;
}

std::shared_ptr<Material>
MaterialFactory::Load(const std::string& name, 
  float shininess,
  float reflectiveness,
  std::shared_ptr<Texture> diff,
  std::shared_ptr<Texture> spec) {

  shared_ptr<Material> m = Load(name);
  //m->shininess = shininess;
  //m->reflectivity = reflectiveness;
  m->diffuse = diff;
  m->specular = spec;

  return m;
}

std::shared_ptr<Material>
Bored::Render::MaterialFactory::Load(const Material& mat) {
  shared_ptr<Material> m = Find(mat.name);
  if (!m) {
    m = make_shared<Material>(mat);
    storage.push_back(m);
  }

  return m;
}

std::shared_ptr<Material>
Bored::Render::MaterialFactory::Load(const std::string& name) {
  shared_ptr<Material> m = Find(name);
  if (!m) {
    m = make_shared<Material>(name);
    storage.push_back(m);
  }

  return m;
}

/* ===================== Helper functions ===========================*/
void GetMaterials(const aiScene* scene, std::vector<Material>& mats) {
  mats.assign(scene->mNumMaterials, Material());

  for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
    aiMaterial* mat = scene->mMaterials[i];
    mats[i].name = std::string(scene->mMaterials[i]->GetName().C_Str()) +
      std::to_string(i);
    aiColor3D diff, spec;
    float opac;
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
    mat->Get(AI_MATKEY_OPACITY, opac);
    //mat->Get<float>(AI_MATKEY_REFLECTIVITY, ans[i].reflectivity);
    //mat->Get<float>(AI_MATKEY_SHININESS, ans[i].shininess);
    // Not populate textures yet. Will do in the main function
  }
}

void GetTextures(const aiScene* scene, std::vector<Texture>& texs) {
  texs.assign(scene->mNumTextures, Texture());

  for (unsigned int i = 0; i < scene->mNumTextures; i++) {
    aiTexture* tex = scene->mTextures[i];
    texs[i].name = tex->mFilename.C_Str();
    texs[i].width = tex->mWidth;
    texs[i].height = tex->mHeight;
    texs[i].bpp = 4;
    texs[i].data = (unsigned char*)tex->pcData;
  }
}

void GetMeshes(const aiScene* scene, std::vector<Mesh>& meshes) {
  meshes.assign(scene->mNumMeshes, Mesh());
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    aiMesh* me = scene->mMeshes[i];
    meshes[i].pos.assign(me->mNumVertices, glm::vec3());
    meshes[i].uvs.assign(me->mNumVertices, glm::vec2());
    meshes[i].norms.assign(me->mNumVertices, glm::vec3());
    meshes[i].name = "Mesh" + std::to_string(i);

    for (unsigned int j = 0; j < me->mNumVertices; j++)
      meshes[i].pos[j] = { me->mVertices[j].x, me->mVertices[j].y,
                             me->mVertices[j].z };

    // Only support 1 texture coordinate set for now
    if (me->HasTextureCoords(0)) {
      for (unsigned int j = 0; j < me->mNumVertices; j++)
        meshes[i].uvs[j] = { me->mTextureCoords[0][j].x,
                              me->mTextureCoords[0][j].y };
    }

    if (me->HasNormals())
      for (unsigned int j = 0; j < me->mNumVertices; j++)
        meshes[i].norms[j] = { me->mNormals[j].x, me->mNormals[j].y,
                                me->mNormals[j].z };

    for (unsigned int j = 0; j < me->mNumFaces; j++) {
      aiFace f = me->mFaces[j];
      for (int k = 0; k < f.mNumIndices; k++)
        meshes[i].indices.push_back(f.mIndices[k]);
    }
  }
}

void ProcessScene(const aiScene* scene,
  std::vector<Material>& mats,
  std::vector<Texture>& texs,
  std::vector<Mesh>& meshes
) {
  if (scene->HasMaterials())
    GetMaterials(scene, mats);
  if (scene->HasTextures())
    GetTextures(scene, texs);
  if (scene->HasMeshes())
    GetMeshes(scene, meshes);
}

/* ============== These ones are exposed to the client ============= */
Bored::Render::Model Bored::Helper::Load(const std::string& file) {
  Assimp::Importer importer;

  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(
    file, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
    aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

  // If the import failed, report it
  if (nullptr == scene) {
    logger->warn(std::string("Cannot load model.\n")
      + importer.GetErrorString());
    return Model(); // TODO return a cube
  }

  std::vector<Material> mats;
    std::vector<Texture> texs;
    std::vector<Mesh> meshes;
  ProcessScene(scene, mats, texs, meshes);

  // Push texs into registries
  for (auto& tex : texs) {
    tex.name = scene->GetShortFilename(file.c_str()) + tex.name;
    TextureFactory::Load(tex);
  }

  // Push meshes into registries
  for (auto& mesh : meshes) {
    mesh.name = scene->GetShortFilename(file.c_str()) + mesh.name;
    MeshFactory::Load(mesh);
  }

  // Populate textures into mats
  for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
    aiMaterial* aiMat = scene->mMaterials[i];
    Material& myMat = mats[i];

    aiString path;
    std::shared_ptr<Texture> tex;
    if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path, 0, 0, 0, 0, 0) ==
      AI_SUCCESS) {
      tex = TextureFactory::Load(path.C_Str());
    }
    else {
      aiColor3D diff;
      aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
      unsigned char color[4] = { 0xff * diff.r, 0xff * diff.g, 0xff * diff.b, 0xff };
      tex = TextureFactory::Load(myMat.name + "Diffuse", 1, 1, 4, color);
    }
    myMat.diffuse = tex;

    if (aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path, 0, 0, 0, 0, 0) ==
      AI_SUCCESS) {
      tex = TextureFactory::Load(path.C_Str());
    }
    else {
      aiColor3D spec;
      aiMat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
      unsigned char color[4] = { 0xff * spec.r, 0xff * spec.g, 0xff * spec.b, 0xff };
      tex = TextureFactory::Load(myMat.name + "Specular", 1, 1, 4, color);
    }
    myMat.specular = tex;

  // Pushes mats into registries
    MaterialFactory::Load(myMat);
  }


  // Construct a model from those things and return that model
  // map mat to mesh
  Model m;
  for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
    aiMesh* me = scene->mMeshes[i];
    me->mMaterialIndex;
    m.renderables.push_back({ MeshFactory::Load(meshes[i].name),
                             MaterialFactory::Load(mats[i].name) });
  }
  // client will change mats, meshes, texture later.
  return m;
}
