#include "RenderUtil.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Bored
{
namespace Render
{
glm::mat4 OrthoProjector::GetMat() const
{
    return glm::ortho(l, r, b, t, 0.1f, 100.0f);
}

glm::mat4 PerspProjector::GetMat() const
{
    return glm::perspective(glm::radians(fov), float(w) / float(h), zNear, zFar);
}

glm::vec3 Camera::GetDir() const
{
    float z = glm::sin(glm::radians(pitch));
    float x = glm::cos(glm::radians(yaw)) * glm::abs(glm::cos(glm::radians(pitch)));
    float y = glm::sin(glm::radians(yaw)) * glm::abs(glm::cos(glm::radians(pitch)));
    return {x, y, z};
}

glm::mat4 Camera::GetViewMat(const glm::vec3& pos) const
{
    auto dir = GetDir();
    return glm::lookAt(pos, pos + dir, up);
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

void GetTextures(const aiScene* scene, std::vector<std::shared_ptr<ITexture>>& texs
                 //, Render::Context* renderContext
)
{
    for (unsigned int i = 0; i < scene->mNumTextures; i++)
    {
        aiTexture* tex = scene->mTextures[i];
        // std::shared_ptr<ITexture> myTex = renderContext->CreateTexture();
        std::shared_ptr<ITexture> myTex = std::make_shared<CPUTexture>();
        myTex->SubData(tex->mWidth, tex->mHeight, 4, tex->pcData);
        myTex->_name = tex->mFilename.C_Str();
        texs.push_back(myTex);
    }
}

void GetMeshes(const aiScene* scene, std::vector<std::shared_ptr<IMesh>>& meshes)
{
    // meshes.assign(scene->mNumMeshes, Mesh());
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh* me = scene->mMeshes[i];
        std::shared_ptr<IMesh> myMesh = std::make_shared<CPUMesh>();
        // myMesh->subPos(std::vector<glm::vec3>());
        std::vector<glm::vec3> pos(me->mNumVertices, glm::vec3());
        std::vector<glm::vec2> uvs(me->mNumVertices, glm::vec2());
        std::vector<glm::vec3> norms(me->mNumVertices, glm::vec3());
        std::vector<unsigned int> indices;

        myMesh->name = "Mesh" + std::to_string(i);

        for (unsigned int j = 0; j < me->mNumVertices; j++)
            pos[j] = {me->mVertices[j].x, me->mVertices[j].y, me->mVertices[j].z};

        // Only support 1 texture coordinate set for now
        if (me->HasTextureCoords(0))
        {
            for (unsigned int j = 0; j < me->mNumVertices; j++)
                uvs[j] = {me->mTextureCoords[0][j].x, me->mTextureCoords[0][j].y};
        }

        if (me->HasNormals())
            for (unsigned int j = 0; j < me->mNumVertices; j++)
                norms[j] = {me->mNormals[j].x, me->mNormals[j].y, me->mNormals[j].z};

        for (unsigned int j = 0; j < me->mNumFaces; j++)
        {
            aiFace f = me->mFaces[j];
            for (int k = 0; k < f.mNumIndices; k++)
                indices.push_back(f.mIndices[k]);
        }

        myMesh->subPos(pos);
        myMesh->subUVs(uvs);
        myMesh->subNorms(norms);
        myMesh->subIndices(indices);
        meshes.push_back(myMesh);
    }
}

void ProcessScene(const aiScene* scene, std::vector<std::shared_ptr<Material>>& mats,
                  std::vector<std::shared_ptr<ITexture>>& texs, std::vector<std::shared_ptr<IMesh>>& meshes)
{
    if (scene->HasMaterials())
        GetMaterials(scene, mats);
    if (scene->HasTextures())
        GetTextures(scene, texs);
    if (scene->HasMeshes())
        GetMeshes(scene, meshes);
}

std::shared_ptr<Model> LoadModel(const std::string& file)
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
    std::vector<std::shared_ptr<ITexture>> texs;
    std::vector<std::shared_ptr<IMesh>> meshes;
    ProcessScene(scene, mats, texs, meshes
                 //, context
    );

    // Push texs into registries
    for (auto& tex : texs)
    {
        tex->_name = scene->GetShortFilename(file.c_str()) + tex->_name;
        // LoadTexture(tex);
    }

    // Push meshes into registries
    for (auto& mesh : meshes)
    {
        mesh->name = scene->GetShortFilename(file.c_str()) + mesh->name;
        // LoadMesh(mesh);
    }

    // Populate textures into mats
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* aiMat = scene->mMaterials[i];
        std::shared_ptr<Material> myMat = mats[i];

        aiString path;
        std::shared_ptr<ITexture> tex;
        if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path, 0, 0, 0, 0, 0) == AI_SUCCESS)
        {
            // tex = LoadTexture(path.C_Str());
        }
        else
        {
            aiColor3D diff;
            aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diff);
            unsigned char color[4] = {static_cast<unsigned char>(0xff * diff.r),
                                      static_cast<unsigned char>(0xff * diff.g),
                                      static_cast<unsigned char>(0xff * diff.b), 0xff};
            // tex = LoadTexture(myMat->name + "Diffuse", 1, 1, 4, color);
        }
        myMat->diffuse = tex;

        if (aiMat->GetTexture(aiTextureType_SPECULAR, 0, &path, 0, 0, 0, 0, 0) == AI_SUCCESS)
        {
            // tex = LoadTexture(path.C_Str());
        }
        else
        {
            aiColor3D spec;
            aiMat->Get(AI_MATKEY_COLOR_SPECULAR, spec);
            unsigned char color[4] = {static_cast<unsigned char>(0xff * spec.r),
                                      static_cast<unsigned char>(0xff * spec.g),
                                      static_cast<unsigned char>(0xff * spec.b), 0xff};
            // tex = LoadTexture(myMat->name + "Specular", 1, 1, 4, color);
        }
        myMat->specular = tex;

        // Pushes mats into registries
        // LoadMaterial(myMat);
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
} // namespace Render
} // namespace Bored
