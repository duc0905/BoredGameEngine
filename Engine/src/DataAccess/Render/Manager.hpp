#pragma once

#include <memory>
#include "EngineConfig.h"

#if RENDER_API == OGL_API // need to include glad before glfw.
#include <glad/glad.h>
#endif

#include "../../Adapter/Render.h"
#include "../../Adapter/Window.h"

namespace Bored
{
namespace Render
{

/* *
 * Abstract factory to create Render adapters
 * Also a singleton
 * */
class Manager
{
  private:
    Manager();

  public:
    ~Manager() = default;

    static Manager& GetInstance();
    // void UseRenderContext(Bored::Window::Window* w);

    std::shared_ptr<VertexArray> CreateVAO();
    std::shared_ptr<VertexBuffer> CreateVBO();
    std::shared_ptr<IndexBuffer> CreateIBO();

    std::shared_ptr<RenderBuffer> CreateRBO();
    std::shared_ptr<FrameBuffer> CreateFBO();

    std::shared_ptr<ITexture> CreateTexture2D();
    std::shared_ptr<ShaderProgram> CreateShaderProgram();

  private:
    static std::unique_ptr<Manager> instance_;
    bool isInit = false;

    // Context* context;
};
} // namespace Render
} // namespace Bored
