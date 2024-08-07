#include "Renderer.hpp"
#include "RenderUtil.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>

namespace Bored
{
namespace Frontend
{

using namespace Render;
Renderer::Renderer(Render::Context* c) : context(c)
{
}

std::shared_ptr<Render::Texture> Renderer::GetMainColorTexture()
{
    return context->GetActiveFrameBuffer().GetColorTexture();
}

void Renderer::BindFramebuffer()
{
    context->GetActiveFrameBuffer().Bind();
}

void Renderer::SetViewport(int l, int b, int r, int t)
{
    context->SetViewport(l, b, r, t);
}

void Renderer::SetClearColor(const glm::vec4& c)
{
    clearColor = c;
}

void Renderer::SetClearColor(const glm::vec3& c)
{
    SetClearColor(glm::vec4(c, 1.0f));
}

void Renderer::Clear()
{
    context->ClearFrameBuffer(clearColor);
}

void Renderer::SetProjector(std::shared_ptr<Render::Projector> projector)
{
    // _projector = projector;
}

void Renderer::SetCamera(std::shared_ptr<Render::Camera> camera)
{
    // _camera = camera;
}

void Renderer::DrawActiveScene()
{
    // Get Camera
    //
    // Get View matrix
    //
    // Get model matrix
}

std::shared_ptr<Render::Texture> Renderer::LoadTexture(const std::string& path)
{
    int w, h, bpp;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.c_str(), &w, &h, &bpp, 4);

    if (data)
        return LoadTexture(path, (unsigned int)w, (unsigned int)h, (unsigned int)bpp, data);

    std::cout << "Failed to load file " << path << std::endl;
    unsigned char white[4] = {0xff, 0xff, 0xff, 0xff};
    auto tex = LoadTexture(path, 1, 1, 4, white);

    _textureRegistry.insert({path, tex});
    return tex;
}

std::shared_ptr<Render::Texture> Renderer::LoadTexture(std::shared_ptr<Render::Texture> tex)
{
    if (_textureRegistry.find(tex->_name) == _textureRegistry.end())
        _textureRegistry.insert({tex->_name, tex});

    return tex;
}

std::shared_ptr<Render::Texture> Renderer::LoadTexture(const std::string& name, unsigned int w, unsigned int h,
                                                       unsigned int bpp, unsigned char* data)
{
    auto tex = context->CreateTexture();

    tex->SubData(w, h, bpp, data);
    tex->_name = name;

    _textureRegistry.insert({name, tex});
    return tex;
}

std::shared_ptr<Render::Texture> Renderer::GetTexture(const std::string& name)
{
    if (_textureRegistry.find(name) == _textureRegistry.end())
        return nullptr;
    return _textureRegistry[name];
}

std::shared_ptr<Render::Mesh> Renderer::LoadMesh(std::shared_ptr<Render::Mesh> mesh)
{
    _meshRegistry.insert({mesh->name, mesh});
    return mesh;
}
std::shared_ptr<Render::Mesh> Renderer::GetMesh(const std::string& name)
{
    if (_meshRegistry.find(name) == _meshRegistry.end())
        return nullptr;
    return _meshRegistry[name];
}

std::shared_ptr<Render::Material> Renderer::LoadMaterial(std::shared_ptr<Render::Material> mat)
{
    _materialRegistry.insert({mat->name, mat});
    return mat;
}
std::shared_ptr<Render::Material> Renderer::GetMaterial(const std::string& name)
{
    if (_materialRegistry.find(name) == _materialRegistry.end())
        return nullptr;
    return _materialRegistry[name];
}

void GetMaterials(const aiScene* scene, std::vector<std::shared_ptr<Material>>& mats)
{
    // mats.assign(scene->mNumMaterials, Material());

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* mat = scene->mMaterials[i];
        std::shared_ptr<Material> myMat = std::make_shared<Material>();

        myMat->name = std::string(mat->GetName().C_Str()) + std::to_string(i);
        // TODO:
        // aiColor3D diff, spec;
        // float opac;
        // mat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
        // mat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
        // mat->Get(AI_MATKEY_OPACITY, opac);
        // mat->Get<float>(AI_MATKEY_REFLECTIVITY, ans[i].reflectivity);
        // mat->Get<float>(AI_MATKEY_SHININESS, ans[i].shininess);
        // Not populate textures yet. Will do in the main function
        mats.push_back(myMat);
    }
}

void GetTextures(const aiScene* scene, std::vector<std::shared_ptr<Texture>>& texs, Render::Context* renderContext)
{
    for (unsigned int i = 0; i < scene->mNumTextures; i++)
    {
        aiTexture* tex = scene->mTextures[i];
        std::shared_ptr<Texture> myTex = renderContext->CreateTexture();
        myTex->SubData(tex->mWidth, tex->mHeight, 4, tex->pcData);
        myTex->_name = tex->mFilename.C_Str();
        texs.push_back(myTex);
    }
}

void GetMeshes(const aiScene* scene, std::vector<std::shared_ptr<Mesh>>& meshes)
{
    // meshes.assign(scene->mNumMeshes, Mesh());
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* me = scene->mMeshes[i];
        std::shared_ptr<Mesh> myMesh = std::make_shared<Mesh>();
        myMesh->pos.assign(me->mNumVertices, glm::vec3());
        myMesh->uvs.assign(me->mNumVertices, glm::vec2());
        myMesh->norms.assign(me->mNumVertices, glm::vec3());
        myMesh->name = "Mesh" + std::to_string(i);

        for (unsigned int j = 0; j < me->mNumVertices; j++)
            myMesh->pos[j] = {me->mVertices[j].x, me->mVertices[j].y, me->mVertices[j].z};

        // Only support 1 texture coordinate set for now
        if (me->HasTextureCoords(0))
        {
            for (unsigned int j = 0; j < me->mNumVertices; j++)
                myMesh->uvs[j] = {me->mTextureCoords[0][j].x, me->mTextureCoords[0][j].y};
        }

        if (me->HasNormals())
            for (unsigned int j = 0; j < me->mNumVertices; j++)
                myMesh->norms[j] = {me->mNormals[j].x, me->mNormals[j].y, me->mNormals[j].z};

        for (unsigned int j = 0; j < me->mNumFaces; j++)
        {
            aiFace f = me->mFaces[j];
            for (int k = 0; k < f.mNumIndices; k++)
                myMesh->indices.push_back(f.mIndices[k]);
        }

        meshes.push_back(myMesh);
    }
}

void ProcessScene(const aiScene* scene, std::vector<std::shared_ptr<Material>>& mats,
                  std::vector<std::shared_ptr<Texture>>& texs, std::vector<std::shared_ptr<Mesh>>& meshes,
                  Render::Context* context)
{
    if (scene->HasMaterials())
        GetMaterials(scene, mats);
    if (scene->HasTextures())
        GetTextures(scene, texs, context);
    if (scene->HasMeshes())
        GetMeshes(scene, meshes);
}

std::shared_ptr<Render::Model> Renderer::LoadModel(const std::string& file)
{
    Assimp::Importer importer;

    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // probably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile(file, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                                       aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

    // If the import failed, report it
    if (nullptr == scene)
    {
        std::cerr << "Error while loading model " << importer.GetErrorString() << std::endl;
        // logger->warn(std::string("Cannot load model.\n")
        //   + importer.GetErrorString());
        return std::make_shared<Model>(); // TODO return a cube
    }

    std::vector<std::shared_ptr<Material>> mats;
    std::vector<std::shared_ptr<Texture>> texs;
    std::vector<std::shared_ptr<Mesh>> meshes;
    ProcessScene(scene, mats, texs, meshes, context);

    // Push texs into registries
    for (auto& tex : texs)
    {
        tex->_name = scene->GetShortFilename(file.c_str()) + tex->_name;
        LoadTexture(tex);
    }

    // Push meshes into registries
    for (auto& mesh : meshes)
    {
        mesh->name = scene->GetShortFilename(file.c_str()) + mesh->name;
        LoadMesh(mesh);
    }

    // Populate textures into mats
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* aiMat = scene->mMaterials[i];
        std::shared_ptr<Material> myMat = mats[i];

        aiString path;
        std::shared_ptr<Texture> tex;
        if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path, 0, 0, 0, 0, 0) == AI_SUCCESS)
        {
            tex = LoadTexture(path.C_Str());
        }
        else
        {
            aiColor3D diff;
            aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
            unsigned char color[4] = {static_cast<unsigned char>(0xff * diff.r),
                                      static_cast<unsigned char>(0xff * diff.g),
                                      static_cast<unsigned char>(0xff * diff.b), 0xff};
            tex = LoadTexture(myMat->name + "Diffuse", 1, 1, 4, color);
        }
        myMat->diffuse = tex;

        if (aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path, 0, 0, 0, 0, 0) == AI_SUCCESS)
        {
            // tex = LoadTexture(path.C_Str());
            tex = LoadTexture(path.C_Str());
        }
        else
        {
            aiColor3D spec;
            aiMat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
            unsigned char color[4] = {static_cast<unsigned char>(0xff * spec.r),
                                      static_cast<unsigned char>(0xff * spec.g),
                                      static_cast<unsigned char>(0xff * spec.b), 0xff};
            tex = LoadTexture(myMat->name + "Specular", 1, 1, 4, color);
        }
        myMat->specular = tex;

        // Pushes mats into registries
        LoadMaterial(myMat);
    }

    // Construct a model from those things and return that model
    // map mat to mesh
    std::shared_ptr<Model> m = std::make_shared<Model>();
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* me = scene->mMeshes[i];
        // me->mMaterialIndex;
        m->renderables.push_back({meshes[i], mats[i]});
    }
    // client will change mats, meshes, texture later.
    return m;
}
} // namespace Frontend
} // namespace Bored
