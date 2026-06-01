#pragma once

#include "Components/FlatComponent.hpp"
#include "Scene/Scene.hpp"
#include "Utils/Font.hpp"

struct GlyphImage : public Bored::Element2D::FlatElement {
  void Render(std::shared_ptr<I_Texture2D> texture) override;

  std::shared_ptr<Bored::Font> font;
  unsigned short glyph_idx;
};

class Scene2D : public Bored::Scene {
public:
  Scene2D(const std::string &font_filepath, unsigned short glyph_idx);

  virtual void BuildScene() override;

  virtual void AfterBuildScene() override;

private:
  std::shared_ptr<Bored::Font> font;
  unsigned short glyph_idx;
};
