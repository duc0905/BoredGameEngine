#include "Scene2D.hpp"
#include "Components/FlatComponent.hpp"
#include "Systems/Input/IOService.hpp"
#include "Systems/Input/InputSystem.hpp"
#include "Systems/Renderer2D/CPU_Renderer2D.hpp"
#include "Utils/AssetManager.hpp"

Scene2D::Scene2D(const std::string &font_filepath, unsigned short glyph_idx) {
  context.io = std::make_shared<Bored::IOService>(800, 600);

  std::shared_ptr<Bored::CPU_Renderer2D> renderer =
      std::make_shared<Bored::CPU_Renderer2D>(800, 600);
  systems.push_back(renderer);

  // Setup input system
  std::shared_ptr<Bored::Input> input =
      std::make_shared<Bored::Input>(*context.io);
  systems.push_back(input);

  Bored::AssetManager &am = Bored::AssetManager::GetInstance();

  font = am.LoadFontTTF(font_filepath);
  this->glyph_idx = glyph_idx;
}

void Scene2D::BuildScene() {
  root = CreateNode();

  std::shared_ptr<GlyphImage> el = std::make_shared<GlyphImage>();
  el->font = font;
  el->glyph_idx = glyph_idx;

  root->AddComponent<Bored::Element2D::FlatComponent>(el);

  {
    auto &g = font->glyf_table.glyphs[glyph_idx];
    auto &[n, xMin, yMin, xMax, yMax] = g.desc;
    std::cout << "Glyph desc: " << n << " " << xMin << " " << yMin << " "
              << xMax << " " << yMax << std::endl;
  }
}

void Scene2D::AfterBuildScene() {}

// TODO:
// - Render contour
// - Check if a pixel is inside or outside the stroke
void GlyphImage::Render(std::shared_ptr<I_Texture2D> texture) {
  auto &g = font->glyf_table.glyphs[glyph_idx];
  auto tex = texture->GetData();
  auto tex_size = texture->GetSize();
  auto bpp = texture->GetBPP();

  auto &[n, xMin, yMin, xMax, yMax] = g.desc;
  int glyph_size_x = xMax - xMin;
  int glyph_size_y = yMax - yMin;

  // Scale to fit the screen
  float s_x = (float)(tex_size.x - 100) / (float)glyph_size_x;
  float s_y = (float)(tex_size.y - 100) / (float)glyph_size_y;
  float s = std::min(s_x, s_y);

  for (uint i = 0; i < g.points.size(); i++) {
    auto &point = g.points[i];
    bool on_curve = g.on_curves[i / 8] & (1 << (i % 8));
    std::byte color[4] = {std::byte(0), std::byte(0), std::byte(0),
                          std::byte(0)};

    int x = (point.x + xMin) * s + 50;
    int y = (point.y + yMin) * s + 50;

    color[3] = std::byte(255);
    if (on_curve) {
      color[2] = std::byte(255);
    } else {
      color[0] = std::byte(255);
    }

    for (int i = -4; i < 4; i++) {
      for (int j = -4; j < 4; j++) {
        unsigned int idx = ((y + i) * tex_size.x + (x + j)) * bpp;
        for (int channel = 0; channel < bpp; channel++) {
          tex[idx + channel] = color[channel];
        }
      }
    }
  }

  texture->WriteData(tex, tex_size, bpp);
}
