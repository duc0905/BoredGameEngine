#pragma once
#include "SubWindow.h"

namespace Bored
{
namespace Editor
{
class GameScreen : public SubWindow
{
  public:
    // Inherited via Window
    void OnInit() override;
    void OnUpdate() override;
    void OnShutdown() override;

  public:
    GameScreen();
    ~GameScreen();
  private:
};

} // namespace Editor
} // namespace Bored
