#pragma once
#include "SubWindow.hpp"

class GameScreen : public SubWindow
{
  public:
    GameScreen(Manifest& game);
    ~GameScreen();

    // Inherited via Window
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;
    void DrawContent() override;
};
