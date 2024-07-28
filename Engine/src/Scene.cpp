#include "Scene.hpp"
#include <memory>
#include <iostream>

namespace Bored
{

ActorManager* Module::GetActorManager()
{
    if (_scene)
        return &_scene->_actorManager;
    else
        return nullptr;
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

// void Scene::Run() {
//     for (auto mod : mods) {
//         mod->OnSetup();
//     }
//
//     bool isRunning = true;
//     double dt = 0;
//     std::chrono::steady_clock::time_point start, end;
//
//     while (isRunning) {
//         start = std::chrono::steady_clock::now();
//         for (auto mod : mods) {
//             end = std::chrono::steady_clock::now();
//             dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//             isRunning &= mod->OnUpdate(dt);
//         }
//     }
//
//     for (auto mod : mods) {
//         mod->OnShutdown();
//     }
// }

bool Scene::OnUpdate(double dt)
{
    bool isRunning = true;

    for (auto mod : m_mods)
    {
        try
        {
            isRunning &= mod->OnUpdate(dt);
        }
        catch (std::exception e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    return isRunning;
}
} // namespace Bored
