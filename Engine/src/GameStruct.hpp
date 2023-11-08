#pragma once
#include <memory>
#include <vector>
#include "Adapter/Window.h"

#define MAX_WINDOWS 4

namespace Bored
{
  struct Game {
    // TODO design a way to have multiple windows
    std::vector<Window*> windows;
    short num_windows = 0;

    short AddWindow(Window* w) {
      if (num_windows >= MAX_WINDOWS)
        throw std::exception("Cannot have more than %d windows per game!", MAX_WINDOWS);
      
      windows.push_back(w);
      return windows.size();
    }

    Window* GetWindow(void* w) {
      for (auto window: windows)
        if (window->GetNativeWindow() == w)
          return window;
        return nullptr;
    }
  };

  Game game;
} // namespace Bored
