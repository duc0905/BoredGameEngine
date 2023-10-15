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
void GameScreen::OnInit()
{
}
void GameScreen::OnUpdate()
{
    ImGui::Text("GameScreen");
    //ImGui::Image((void*)5, ImVec2(800, 600));
}
void GameScreen::OnShutdown()
{
}

} // namespace Editor
} // namespace Bored
