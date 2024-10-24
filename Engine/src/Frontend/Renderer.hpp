#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "../Adapter/Render.h"
#include "../Scene.hpp"
#include "RenderUtil.hpp"
#include "CPU/Factory.hpp"

namespace Bored
{
namespace Frontend
{
/**
 * @brief Use the context to create render outputs
 * TODO: Make this abstract to create normal renderer, race tracing,...
 *
 */
class Renderer
{
  public:
    Renderer(Render::Context* c);

    std::shared_ptr<Render::NativeTexture> GetMainColorTexture();

    void BindFramebuffer();

    void SetViewport(int l, int b, int r, int t);
    void SetClearColor(const glm::vec4& c);
    void SetClearColor(const glm::vec3& c);
    void Clear();

  public:
    void UseShaderProgram(std::shared_ptr<Bored::Render::ShaderProgram> p_shader);
    void DrawActiveScene();
    void OnSwitchScene(std::shared_ptr<Bored::Scene> p_scene);

  public:
    // TODO: Just a temporary fix. We should know which factory to use since we have render context
    void UseFactory(Render::IFactory* p_factory)
    {
        m_factory.reset(p_factory);
    }

    // TODO: Move these into factory
    // public:
    //   std::shared_ptr<Render::ITexture> LoadTexture(const std::string& path);
    //   std::shared_ptr<Render::ITexture> LoadTexture(std::shared_ptr<Render::ITexture> tex);
    //   std::shared_ptr<Render::ITexture> LoadTexture(const std::string& name, unsigned int w, unsigned int h,
    //                                                 unsigned int bpp, unsigned char* data);
    //   std::shared_ptr<Render::ITexture> GetTexture(const std::string& path);
    //
    //   std::shared_ptr<Render::IMesh> LoadMesh(std::shared_ptr<Render::IMesh> mesh);
    //   std::shared_ptr<Render::IMesh> GetMesh(const std::string& name);
    //
    //   std::shared_ptr<Render::Material> LoadMaterial(std::shared_ptr<Render::Material> mat);
    //   std::shared_ptr<Render::Material> GetMaterial(const std::string& name);
  private:
    std::shared_ptr<Bored::Scene> m_activeScene;

  private:
    Render::Context* context = nullptr;
    // TODO: merge context into factory
    std::unique_ptr<Render::IFactory> m_factory = nullptr;
    // TODO: just a temporary solution for now
    Render::CPU::Factory m_cpuFactory;
    std::shared_ptr<Render::ShaderProgram> m_shaderProgram;

  private:
    glm::vec4 clearColor;
    //
    // private:
    //   std::map<std::string, std::shared_ptr<Render::Model>> m_modelRegistry;
    //   std::map<std::string, std::shared_ptr<Render::ITexture>> m_textureRegistry;
    //   std::map<std::string, std::shared_ptr<Render::Material>> m_materialRegistry;
    //   std::map<std::string, std::shared_ptr<Render::IMesh>> m_meshRegistry;
};
} // namespace Frontend
} // namespace Bored
