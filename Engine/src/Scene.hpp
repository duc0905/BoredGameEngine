#pragma once
#include "ECS/ActorManager.hpp"
#include "Utils/DLLLoader/DLLLoader.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace Bored
{

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
    virtual bool OnUpdate(double dt) = 0;
    virtual void OnShutdown() = 0;

  protected:
    ActorManager* GetActorManager();

  private:
    void Attach(Scene* scene)
    {
        _scene = scene;
    }

  protected:
    Scene* _scene;
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

    // void Run();
    bool OnUpdate(double dt);

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

  private:
    std::weak_ptr<Scene> _this;
    std::vector<std::shared_ptr<Module>> m_mods;
    std::vector<std::shared_ptr<Util::DLLoader>> m_loaders;
    Bored::ActorManager _actorManager;
};
} // namespace Bored
