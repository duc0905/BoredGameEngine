#pragma once
#include "../GameLoop.hpp"
#include "Render.h"
#include "../Frontend/Renderer.hpp"

namespace Bored
{
class Window : public Module
{
  public:
    virtual void SetWidth(unsigned int) = 0;
    virtual void SetHeight(unsigned int) = 0;
    virtual void SetTitle(const std::string&) = 0;
    virtual void SetFullscreen(bool) = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;
};
} // namespace Bored