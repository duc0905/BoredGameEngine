#include "Renderer.hpp"
#include "RenderUtil.hpp"
#include <exception>
#include <iostream>
// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../ECS/Components/Transform.hpp"
#include "../ECS/Components/Camera.hpp"
#include <glm/fwd.hpp>

namespace Bored
{
namespace Frontend
{

using namespace Render;
Renderer::Renderer(Render::Context* c) : context(c)
{
}

std::shared_ptr<Render::ITexture> Renderer::GetMainColorTexture()
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

void Renderer::UseShaderProgram(std::shared_ptr<Bored::Render::ShaderProgram> p_shader)
{
    if (p_shader->IsLinked())
    {
        m_shaderProgram = p_shader;
    }
    else
    {
        throw std::exception("Shader is not complete");
    }
}

/**
 * Helper to get View and Project matrices
 *
 * Description.
 */
glm::mat4 GetMatrices(ECS::Transform& transform, ECS::Camera& camera)
{
    // TODO: implement
    glm::vec3 eye = transform.pos;
    glm::vec3 center = transform.pos + camera.dir;
    glm::vec3 up = camera.up;

    return camera.projector->GetMat() * glm::lookAt(eye, center, up);
}

glm::mat4 GetModelMatrix(ECS::Transform& transform, Render::Model& model)
{
    // TODO: implement
    return glm::mat4(1.0f);
}

void PrintMatrix(glm::mat4 m)
{
    std::cout << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl;
    std::cout << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl;
    std::cout << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl;
    std::cout << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
}

void Renderer::DrawActiveScene()
{
    // Get Camera
    auto camera = m_activeScene->GetActiveCamera();

    // Get MVP matrix
    auto [transformComp, cameraComp] = m_activeScene->GetActorManager().Get<ECS::Transform, ECS::Camera>(camera->id);

    if (transformComp == nullptr || cameraComp == nullptr)
    {
        throw std::exception("Invalid camera actor");
    }

    m_shaderProgram->Bind();
    auto vpMatrix = GetMatrices(*transformComp, *cameraComp);
    m_shaderProgram->SetUniform("VPMatrix", vpMatrix);

    auto view = m_activeScene->GetActorManager().Get<Bored::ECS::Transform, Bored::Render::Model>();

    for (auto [id, trans, model] : view.each())
    {
        auto modelMatrix = GetModelMatrix(trans, model);
        m_shaderProgram->SetUniform("ModelMatrix", modelMatrix);

        for (auto [mesh, material] : model.renderables)
        {
            // std::cout << "Drawing " << mesh->name << std::endl;
            auto pos = mesh->getPos();
            glm::mat4 mvp = vpMatrix * modelMatrix;
            glm::vec4 p0 = mvp * glm::vec4(pos[0], 1.0f);
            glm::vec4 p1 = mvp * glm::vec4(pos[1], 1.0f);
            glm::vec4 p2 = mvp * glm::vec4(pos[2], 1.0f);

            std::cout << "final pos0: " << p0.x << " " << p0.y << " " << p0.z << std::endl;
            std::cout << "final pos1: " << p1.x << " " << p1.y << " " << p1.z << std::endl;
            std::cout << "final pos2: " << p2.x << " " << p2.y << " " << p2.z << std::endl;

            // PrintMatrix(mvp);

            m_shaderProgram->Draw(mesh, material);
            // TODO:
            // context->DrawVertexArray(mesh->m_vaosomehow, m_shaderProgram);
        }
    }
    m_shaderProgram->Unbind();
}

void Renderer::OnSwitchScene(std::shared_ptr<Bored::Scene> p_scene)
{
    auto& am = p_scene->GetActorManager();
    auto models = am.Get<Bored::Render::Model>();

    m_activeScene = p_scene;
    for (auto [id, model] : models.each())
    {
        // TODO: Convert to OGLMesh
    }
}

std::shared_ptr<Render::ITexture> Renderer::LoadTexture(const std::string& path)
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

    m_textureRegistry.insert({path, tex});
    return tex;
}

std::shared_ptr<Render::ITexture> Renderer::LoadTexture(std::shared_ptr<Render::ITexture> tex)
{
    if (m_textureRegistry.find(tex->_name) == m_textureRegistry.end())
        m_textureRegistry.insert({tex->_name, tex});

    return tex;
}

std::shared_ptr<Render::ITexture> Renderer::LoadTexture(const std::string& name, unsigned int w, unsigned int h,
                                                        unsigned int bpp, unsigned char* data)
{
    auto tex = context->CreateTexture();

    tex->SubData(w, h, bpp, data);
    tex->_name = name;

    m_textureRegistry.insert({name, tex});
    return tex;
}

std::shared_ptr<Render::ITexture> Renderer::GetTexture(const std::string& name)
{
    if (m_textureRegistry.find(name) == m_textureRegistry.end())
        return nullptr;
    return m_textureRegistry[name];
}

std::shared_ptr<Render::IMesh> Renderer::LoadMesh(std::shared_ptr<Render::IMesh> mesh)
{
    m_meshRegistry.insert({mesh->name, mesh});
    return mesh;
}
std::shared_ptr<Render::IMesh> Renderer::GetMesh(const std::string& name)
{
    if (m_meshRegistry.find(name) == m_meshRegistry.end())
        return nullptr;
    return m_meshRegistry[name];
}

std::shared_ptr<Render::Material> Renderer::LoadMaterial(std::shared_ptr<Render::Material> mat)
{
    m_materialRegistry.insert({mat->name, mat});
    return mat;
}
std::shared_ptr<Render::Material> Renderer::GetMaterial(const std::string& name)
{
    if (m_materialRegistry.find(name) == m_materialRegistry.end())
        return nullptr;
    return m_materialRegistry[name];
}
} // namespace Frontend
} // namespace Bored
