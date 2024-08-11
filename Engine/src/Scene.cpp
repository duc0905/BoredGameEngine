#include "Scene.hpp"
#include "Adapter/Window.h"
#include <exception>
#include <memory>
#include <iostream>

namespace Bored
{

ActorManager* Module::GetActorManager()
{
    if (m_scene)
        return &m_scene->_actorManager;
    else
        return nullptr;
}

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::OnSetup()
{
    for (auto mod : m_mods)
    {
        try
        {
            mod->OnSetup();
        }
        catch (std::exception e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
}

void Scene::OnSwitchScene()
{
    for (auto mod : m_mods)
    {
        try
        {
            mod->OnSwitchScene();
        }
        catch (std::exception e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
}

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

void Scene::OnShutdown()
{
    for (auto mod : m_mods)
    {
        try
        {
            mod->OnShutdown();
        }
        catch (std::exception e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }
}
} // namespace Bored
