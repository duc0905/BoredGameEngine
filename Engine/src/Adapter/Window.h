#pragma once
#include <memory>
#include "../GameLoop.hpp"
#include "Render.h"
#include "../Frontend/Renderer.hpp"

namespace Bored
{
class Window : public Module
{
  public:
    virtual void SetWidth(int) = 0;
    virtual void SetHeight(int) = 0;
    virtual void SetTitle(const std::string&) = 0;
    virtual void SetFullscreen(bool) = 0;

    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    /** 
     * @brief the content of the window using ImGui
     * Can assume that is being called in a valid window
    **/
    virtual void DrawContent() = 0;

    /**
     * @brief Set the render context to be used by the renderer
     * @param con 
     */
    void UseRenderContext(Render::Context* con)
    {
      renderContext.reset(con);
    }

    Frontend::Renderer& GetRenderer()
    {
      if (!renderer) {
        if (!renderContext) {
          throw std::exception("Must call UseRenderContext before calling GetRenderer");
        }
        renderer.reset(new Frontend::Renderer(renderContext.get()));
      }
      return *renderer;
    }
  private:
    std::unique_ptr<Frontend::Renderer> renderer;
    std::unique_ptr<Render::Context> renderContext;

    // TODO Audio context
};
} // namespace Bored