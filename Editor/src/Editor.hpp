#pragma once

#include <string>

#include "SubWindow.h"
#include "GameManifest/Manifest.hpp"

class Editor
{
    friend class Bored::Editor::SubWindow;
private:
    static std::unique_ptr<Editor> _instance;

    Editor() {}

public:
    ~Editor() {}

    static Editor& GetInstance();

    void Init();
    void Run();
    void Shutdown();

    void OpenProject(const std::string& directory);

    Manifest GetManifest() const { return _game; }

private:
    Bored::GLFW::Window* mainWindow;
    // Sub windows
    std::vector<std::shared_ptr<Bored::Editor::SubWindow>> windows;

    Manifest _game;
    bool _gameLoaded = false;

    bool show_demo_window = false;
};
