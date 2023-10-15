#pragma once
#include "window.h"

namespace Bored
{
namespace Editor
{
class GameScreen : public Window
{
  public:
    // Inherited via Window
    void Init() override;
    void Update() override;
    void Shutdown() override;

  public:
    GameScreen();
    ~GameScreen();
  private:
};

} // namespace Editor
} // namespace Bored
