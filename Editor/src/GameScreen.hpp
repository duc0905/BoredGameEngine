#pragma once
#include "SubWindow.h"
#include "Bored.hpp"

namespace Bored
{
namespace Editor
{
class GameScreen : public SubWindow
{
  public:
    // Inherited via Window
    void OnSetup() override;
    bool OnUpdate(double dt) override;
    void OnShutdown() override;
    void DrawContent() override;

  public:
    GameScreen();
    ~GameScreen();
};

} // namespace Editor
} // namespace Bored
