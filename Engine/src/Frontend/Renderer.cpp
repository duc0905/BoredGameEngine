#include "Renderer.hpp"
#include "RenderUtil.hpp"
#include <iostream>
// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

// void Renderer::SetCamera(std::shared_ptr<ECS::Camera> camera)
// {
//     // _camera = camera;
// }

void Renderer::DrawActiveScene()
{
    // Get Camera
    auto camera = m_activeScene->GetActiveCamera();
    //
    // Get View matrix
    //
    // Get model matrix
}

void Renderer::OnSwitchScene(std::shared_ptr<Bored::Scene> p_scene)
{
    auto& am = p_scene->GetActorManager();
    auto models = am.Get<Bored::Render::Model>();

    for (auto [id, model] : models.each()) {
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
