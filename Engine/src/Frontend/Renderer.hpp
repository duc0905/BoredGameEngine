#pragma once

#include <glm/glm.hpp>
#include <map>
#include <memory>

#include "../ECS/Actor.hpp"
#include "../Adapter/Render.h"
#include "../Scene.hpp"
#include "RenderUtil.hpp"

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

    std::shared_ptr<Render::Texture> GetMainColorTexture();

    void BindFramebuffer();

    void SetViewport(int l, int b, int r, int t);
    void SetClearColor(const glm::vec4& c);
    void SetClearColor(const glm::vec3& c);
    void Clear();

  public:
    void DrawActiveScene();

  public:
    void SetProjector(std::shared_ptr<Render::Projector> projector);
    // TODO: Change camera to be an actor in the scene
    void SetCamera(std::shared_ptr<Render::Camera> camera);

    /*  */
  public:
    std::shared_ptr<Render::Model> LoadModel(const std::string& file);

    std::shared_ptr<Render::Texture> LoadTexture(const std::string& path);
    std::shared_ptr<Render::Texture> LoadTexture(std::shared_ptr<Render::Texture> tex);
    std::shared_ptr<Render::Texture> LoadTexture(const std::string& name, unsigned int w, unsigned int h,
                                                 unsigned int bpp, unsigned char* data);
    std::shared_ptr<Render::Texture> GetTexture(const std::string& path);

    std::shared_ptr<Render::Mesh> LoadMesh(std::shared_ptr<Render::Mesh> mesh);
    std::shared_ptr<Render::Mesh> GetMesh(const std::string& name);

    std::shared_ptr<Render::Material> LoadMaterial(std::shared_ptr<Render::Material> mat);
    std::shared_ptr<Render::Material> GetMaterial(const std::string& name);

  private:
    Render::Context* context;
    glm::vec4 clearColor;

  private:
    // std::shared_ptr<Render::Projector> _projector;
    // std::shared_ptr<Render::Camera> _camera;
    std::shared_ptr<Bored::Scene> _active_scene;

  private:
    std::map<std::string, std::shared_ptr<Render::Model>> _modelRegistry;
    std::map<std::string, std::shared_ptr<Render::Texture>> _textureRegistry;
    std::map<std::string, std::shared_ptr<Render::Material>> _materialRegistry;
    std::map<std::string, std::shared_ptr<Render::Mesh>> _meshRegistry;
};
} // namespace Frontend
} // namespace Bored
