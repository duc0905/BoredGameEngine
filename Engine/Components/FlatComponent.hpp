#pragma once

#include "../Systems/Renderer/I_Texture.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <ostream>

namespace Bored {
struct FlatElement {
  glm::vec2 pos;
  glm::vec2 size;

  virtual void Render(std::shared_ptr<I_Texture2D> texture) = 0;
};

struct Container : public FlatElement {
  glm::vec4 bg_color{0.8f, 0.3f, 0.3f, 1.0f};

  virtual void Render(std::shared_ptr<I_Texture2D> texture) {
    auto tex = texture->GetData();
    auto tex_size = texture->GetSize();
    auto bpp = texture->GetBPP();

    for (int row = std::max((int)pos.y, 0);
         row < std::min((int)(pos.y + size.y), tex_size.y); row++) {
      for (int col = std::max((int)pos.x, 0);
           col < std::min((int)(pos.x + size.x), tex_size.x); col++) {
        unsigned int idx = (row * tex_size.x + col) * bpp;
        for (int channel = 0; channel < bpp; channel++) {
          tex[idx + channel] =
              static_cast<std::byte>(255 * (int)bg_color[channel]);
        }
      }
    }

    texture->WriteData(tex, tex_size, bpp);
  }
};

struct FlatComponent {
  std::shared_ptr<FlatElement> element;
};
} // namespace Bored
