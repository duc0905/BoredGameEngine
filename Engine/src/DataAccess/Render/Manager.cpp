#include "Manager.hpp"
#include <memory>

#if RENDER_API == OGL_API
#include "../../Adapter/OGL.h" // Compile OGL.cpp only if the render api is OGL
#endif

#include "../../Adapter/Render.h"
#include "../../Adapter/Window.h"

namespace Bored
{
namespace Render
{
std::unique_ptr<Manager> Manager::instance_ = nullptr;

Manager::Manager() : isInit(false)
{
}

Manager& Manager::GetInstance()
{
    if (!instance_)
        instance_ = std::unique_ptr<Manager>(new Manager());
    return *instance_;
}

// void Manager::UseRenderContext(Bored::Window::Window* w)
//{
//     context = w->GetRenderContext();
// }

std::shared_ptr<VertexArray> Manager::CreateVAO()
{
#if RENDER_API == OGL_API
    return std::make_shared<OGL::VertexArray>();
#else
    return nullptr;
#endif
}

std::shared_ptr<VertexBuffer> Manager::CreateVBO()
{
#if RENDER_API == OGL_API
    return std::make_shared<OGL::VertexBuffer>();
#else
    return nullptr;
#endif
}

std::shared_ptr<IndexBuffer> Manager::CreateIBO()
{
#if RENDER_API == OGL_API
    return std::make_shared<OGL::IndexBuffer>();
#else
    return nullptr;
#endif
}

std::shared_ptr<RenderBuffer> Manager::CreateRBO()
{
    return nullptr;
}

std::shared_ptr<FrameBuffer> Manager::CreateFBO()
{
    return nullptr;
}
std::shared_ptr<ITexture> Manager::CreateTexture2D()
{
    std::shared_ptr<ITexture> tex = nullptr;
#if RENDER_API == OGL_API
    tex = std::make_shared<OGL::Texture2D>();
#endif

    return tex;
}
std::shared_ptr<ShaderPipeline> Manager::CreateShaderProgram()
{
    return nullptr;
}
} // namespace Render
} // namespace Bored
