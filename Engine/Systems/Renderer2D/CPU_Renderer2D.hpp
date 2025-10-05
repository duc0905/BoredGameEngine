#pragma once

#include "I_Renderer2D.hpp"

namespace Bored {
class CPU_Renderer2D : public I_Renderer2D {
public:
  CPU_Renderer2D(int width, int height);

  virtual void OnUpdate(double dt, Scene &scene);

  virtual std::shared_ptr<I_Texture2D> Render(Scene &scene);

private:
  std::shared_ptr<I_Texture2D> pixels;
};
}; // namespace Bored
