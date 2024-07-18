#pragma once

#include <string>
#include "GameManifest/Manifest.hpp"
#include "SubWindow/SubWindow.hpp"

class Editor
{
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
    std::vector<std::shared_ptr<SubWindow>> windows;

    Manifest _game;
    bool _gameLoaded = false;

    bool show_demo_window = true;
};
