#include "GameScreen.hpp"
#include <imgui.h>
#include <iostream>
#include <memory>

#include "../constants.h"
#include "ActorManager.hpp"

GameScreen::GameScreen(Manifest& game) 
  : SubWindow(game, "Gamescreen", 800, 600)
{
}
GameScreen::~GameScreen()
{
}
void GameScreen::OnSetup()
{
    UseRenderContext(new Bored::Render::OGL::Context());
    GetRenderer().SetClearColor({0.0f, 0.1f, 0.3f, 1.0f});
}

bool GameScreen::OnUpdate(double dt)
{
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
    // auto mod = _game.core_modules[ACTORMANAGER_TOKEN];
    // if (!mod)
    //     std::cerr << "Actor Manager does not exist" << std::endl;
    // else {
    //     auto am = std::dynamic_pointer_cast<Bored::ActorManager>(mod);
    //
    //     // Get actors
    //     // TODO: Implement ModelComp
    //     auto actors = am->Get<Transform /*, ModelComp */>();
    //
    //     r.BindFramebuffer();
    //     for (auto [id, transform /*, model */] : actors.each()) {
    //         // Draw
    //         // r.DrawModel(model, transform);
    //     }
    // }

    auto tex = r.GetMainColorTexture();

    // Draw the colorbuffer from the gamerenderer
    ImGui::Image(tex->GetId(), ImGui::GetContentRegionAvail());
}

void GameScreen::OnShutdown()
{
}
