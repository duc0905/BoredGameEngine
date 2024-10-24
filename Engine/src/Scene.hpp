#pragma once
#include <memory>
#include <utility>
#include <vector>

// #include "Adapter/Window.h"
#include "ECS/Actor.hpp"
#include "ECS/ActorManager.hpp"
#include "Utils/DLLLoader/DLLLoader.hpp"

namespace Bored
{
class Window;
class Scene;

/**
 * A logic processing component of a scene. Have access to the ActorManager (ECS) of its scene
 * Only use Components to process logic
 */
class Module
{
    friend class Scene;

  public:
    virtual void OnSetup() = 0;
    virtual void OnSwitchScene()
    {
    }
    virtual bool OnUpdate(double dt) = 0;
    virtual void OnShutdown() = 0;

  protected:
    ActorManager* GetActorManager();

  private:
    void Attach(Scene* scene)
    {
        m_scene = scene;
    }

  protected:
    Scene* m_scene;
};

/**
 * A scene in the game. Can have modules to process logics.
 */
class Scene
{
    friend class Module;

  public:
    Scene();
    ~Scene();

    /**
     * TODO: Clearly define when these lifecycle methods should be called
     */
    void OnSetup();
    void OnSwitchScene();
    bool OnUpdate(double dt);
    void OnShutdown();

    template <class T, class... Args> std::shared_ptr<T> AddModule(Args&&... args)
    {
        std::shared_ptr<T> m = std::make_shared<T>(std::forward<Args>(args)...);
        m->Attach(this);
        m_mods.push_back(m);
        return m;
    }

    /**
     * modulePath is the path to the dll
     */
    template <class T>
    std::shared_ptr<T> AddDLModule(const std::string& modulePath, const std::string& allocSymbol = CREATE_INSTANCE,
                                   const std::string& deleteSymbl = DELETE_INSTANCE)
    {
        std::shared_ptr<Util::DLLoader> loader = std::make_shared<Util::DLLoader>(modulePath);
        loader->Load();
        std::shared_ptr<T> mod = loader->GetIntance<T>();
        mod->OnUpdate(1000);
        mod->Attach(this);
        m_mods.push_back(mod);
        m_loaders.push_back(loader);
        return mod;
    }

    Bored::ActorManager& GetActorManager()
    {
        return m_actorManager;
    }

  public:
    void UseCamera(std::shared_ptr<Bored::Actor> p_camera);
    std::shared_ptr<Bored::Actor> GetActiveCamera() const;

  private:
    std::weak_ptr<Scene> _this;
    std::vector<std::shared_ptr<Module>> m_mods;
    std::vector<std::shared_ptr<Util::DLLoader>> m_loaders;

    Bored::ActorManager m_actorManager;

  private:
    std::shared_ptr<Bored::Actor> m_activeCamera;
};
} // namespace Bored
