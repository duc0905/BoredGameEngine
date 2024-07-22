#pragma once
#include "ECS/ActorManager.hpp"
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
        _mods.push_back(m);
        return m;
    }

  private:
    std::weak_ptr<Scene> _this;
    std::vector<std::shared_ptr<Module>> _mods;
    Bored::ActorManager _actorManager;
};
} // namespace Bored
