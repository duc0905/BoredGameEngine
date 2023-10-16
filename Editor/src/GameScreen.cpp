#include "GameScreen.hpp"
#include <imgui.h>

namespace Bored
{
namespace Editor
{
GameScreen::GameScreen() 
  : SubWindow("Gamescreen", 800, 600)
{
}
GameScreen::~GameScreen()
{
}
void GameScreen::OnSetup()
{
    UseRenderContext(new Render::OGL::Context());
    GetRenderer().SetClearColor({0.6f, 0.5f, 0.3f, 1.0f});
}

bool GameScreen::OnUpdate(double dt)
{
    // TODO: Update the game logic here
    return true;
}

void GameScreen::DrawContent()
{
    ImGui::Text("GameScreen");

    // Get the gamerenderer
    auto r = GetRenderer();
    r.SetViewport(0, 0, GetWidth(), GetHeight()); // As large as this window
    r.Clear();

    // TODO: Draw actors here
    auto tex = r.GetMainColorTexture();

    // Draw the colorbuffer from the gamerenderer
    ImGui::Image(tex->GetId(), ImGui::GetContentRegionAvail());
}

void GameScreen::OnShutdown()
{
}

} // namespace Editor
} // namespace Bored
