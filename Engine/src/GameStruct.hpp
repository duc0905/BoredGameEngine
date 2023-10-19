#pragma once
#include <memory>
#include "Adapter/Window.h"

#define MAX_WINDOWS 4

namespace Bored
{
  struct Game {
    // TODO design a way to have multiple windows
    Window* window;
  };

  Game game;
} // namespace Bored
