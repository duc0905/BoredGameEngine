#pragma once
#include <memory>
#include <vector>
#include "Adapter/Window.h"

#define MAX_WINDOWS 4

namespace Bored
{
struct Game
{
    // Idea: design a way to have multiple windows
    // Welp, turns out it will make the editor so much harder so nahhhhh
    short num_windows = 0;
    std::vector<Window*> windows;

    // Contains the "logic" part of the game
    Bored::Scene game_logic;

    short AddWindow(Window* w)
    {
        if (num_windows >= MAX_WINDOWS)
            throw std::exception("Cannot have more than %d windows per game!", MAX_WINDOWS);

        windows.push_back(w);
        return windows.size();
    }

    short AddWindow(std::shared_ptr<Window> w)
    {
        return AddWindow(w.get());
    }

    Window* GetWindow(void* w)
    {
        for (auto window : windows)
            if (window->GetNativeWindow() == w)
                return window;
        return nullptr;
    }
};

} // namespace Bored

// extern Bored::Game game;
