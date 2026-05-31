#pragma once

#include <memory>
#include "../I_System.hpp"
#include "../Renderer/I_Texture.hpp"

namespace Bored {
class I_Renderer2D : public I_System {
protected:
  /**
   * Use the scene to render the 2D objects.
   */
  virtual std::shared_ptr<I_Texture2D> Render(Scene& scene) = 0;
};

}; // namespace Bored
