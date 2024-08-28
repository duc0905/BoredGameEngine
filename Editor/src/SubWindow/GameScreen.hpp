#pragma once
#include "SubWindow.hpp"

/**
  * This class is purely for displaying, no logic processing is dealt here
  */
class GameScreen : public SubWindow
{
  public:
    GameScreen(Manifest& game);
    ~GameScreen();

    // Inherited via Window
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;
    void NewFrame() override;
};
