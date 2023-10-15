#include "GameScreen.hpp"
#include <imgui.h>

namespace Bored
{
namespace Editor
{
GameScreen::GameScreen() 
  : Window("Gamescreen", 800, 600)
{
}
GameScreen::~GameScreen()
{
}
void GameScreen::Init()
{
}
void GameScreen::Update()
{
    ImGui::Text("GameScreen");
    //ImGui::Image((void*)5, ImVec2(800, 600));
}
void GameScreen::Shutdown()
{
}

} // namespace Editor
} // namespace Bored
