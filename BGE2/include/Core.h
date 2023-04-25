#pragma once
#define USE_GAME_MODULES                                                       \
  using Bored::renderer;                                                       \
  using Bored::audio;                                                          \
  using Bored::input;                                                          \
  using Bored::actorManager;                                                   \
  using Bored::gamemode;                                                       \
  using Bored::addons;

#include "../pch.h"
#include "Bases.h"
#include "Render/Utils.h"
#include "Camera.h"
// #include "ActorManager.h"
// #include "Render/Utils.h"

// Just define the general interfaces here
namespace Bored
{
  // Input file
  struct Transform
  {
    glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
    glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

    [[nodiscard]] const glm::mat4 GetMat() const;
  };

  /**
   * @brief Holds the logics of the game
   */
  class Gamemode : public Module {};

  /**
   * @brief Provide chrono functionalities
   */
  class Timer : public Module
  {
    std::chrono::system_clock::time_point last_frame;

  public:
    virtual void OnSetup() override
    {
      last_frame = std::chrono::system_clock::now();
    }

    double GetDt() const
    {
      return std::chrono::duration<double>(std::chrono::system_clock::now() -
        last_frame)
        .count();
    }

    virtual bool OnTick(double) override
    {
      last_frame = std::chrono::system_clock::now();
      return false;
    }
  };

  class Actor;
  struct IDToPtr
  {
    std::shared_ptr<Actor> ptr;
    IDToPtr(const IDToPtr& o) { ptr = o.ptr; }
    IDToPtr(std::shared_ptr<Actor> p) : ptr(p) {}
    operator std::shared_ptr<Actor>() { return ptr; }
  };
  /**
   * @brief A special data structure to manage actors effectively
   */
  class ActorManager : public Module
  {
  public:
    entt::registry actor_registry;
    /**
     * @brief Create an actor or subtype of actor
     * @tparam T Actor or subtype of Actor
     * @tparam ...Args
     * @tparam
     * @param ...args params to create actor
     * @return shared pointer to the created actor
     */
    template <typename T, class... Args,
      std::enable_if_t<std::is_base_of_v<Actor, T>, int> = 0>
    std::shared_ptr<T> Create(Args &&...args)
    {
      auto id = actor_registry.create();
      std::shared_ptr<T> a = std::make_shared<T>(args...);
      a->id = id;
      actor_registry.emplace<Transform>(id);
      actor_registry.emplace<IDToPtr>(id, a);
      return a;
    }

    /**
     * @brief Get components of actors with those components
     * @tparam ...Comps
     * @return entt::view of those components
     */
    template <typename... Comps>
    entt::basic_view<entt::entity, entt::exclude_t<>, Comps...> Get()
    {
      return actor_registry.view<Comps...>();
    }

    template <typename... Comps>
    decltype(auto) Get(entt::entity id)
    {
      return actor_registry.get_or_emplace<Comps...>(id);
    }

    template <typename T, class... Args>
    T& AddComponent(entt::entity id, Args &&...args)
    {
      if (!actor_registry.valid(id))
      {
        std::cout << "[Warning]: Adding component " << typeid(T).name()
          << " for invalid actor: " << (int)id << std::endl;
      }

      if (actor_registry.has<T>(id))
      {
        std::cout << "[Warning]: Component '" << typeid(T).name()
          << "' already exist for actor " << (uint64_t)id
          << ". Getting the existed component instead!" << std::endl;

        return actor_registry.get<T>(id);
      }

      return actor_registry.emplace_or_replace<T>(id, std::forward<Args>(args)...);
    }

    template <typename T>
    void RemoveComponent(entt::entity id)
    {
      if (typeid(T) == typeid(Transform))
      {
        std::cout << "[Warning]: Cannot remove Transform component." << std::endl;
        return;
      }

      if (!actor_registry.has<T>(id))
      {
        std::cout << "[Warning]: Actor " << (uint64_t)id << " does not have Component '"
          << typeid(T).name() << "'. Not removing any component!" << std::endl;
        return;
      }

      actor_registry.remove<T>(id);
      return;
    }

    void Delete(entt::entity id);

    // Inherited via Module
    virtual void OnSetup() override {};
    virtual bool OnTick(double dt) override;
    virtual void OnImGui() override;
  };

  // Output classes - depends on the device. Implement these for each platform.
  class Renderer : public Module
  {
  public:
    unsigned int width = 1200, height = 800;
  protected:
    std::shared_ptr<Actor> active_cam;
    std::unique_ptr<Projector> projector =
      //std::make_unique<OrthoProjector>(-1.0f, 1.0f, -1.0f, 1.0f);
      std::make_unique<PerspProjector>(width, height);
    std::vector<std::shared_ptr<Actor>> light_srcs;

    [[nodiscard]] const glm::mat4& GetProjMat() const;
  public:
    void UseCamera(std::shared_ptr<Actor> cam);
  };
  class Audio : public Module {};


  class Input : public Module {
  public:
    // Just for intellisense
    enum Key;
    enum Action;
    #include "InputKey.h"

    /** Context
    * Define a set of actions/state/ranges those are available when this context is active
    * Multiple contexts can be active at once
    * Using Chain of responsibility pattern
    */
    class Context
    {
      friend class Input;
    private:
      std::shared_ptr<Context> next_;
      int priority_ = 1;
      bool isActive_ = false;

      std::map<std::pair<Key, int>, std::string> actionMap_;
      std::map<std::pair<Key, int>, std::pair<std::string, float>> rangeMap_;

      void ResetPriority(int priority);
      void Activate();
      void Deactivate();
    public:
      Context() = default;

      void AddActionMapping(Input::Key key, int mods, const std::string& name);
      void RemoveActionMapping(Input::Key key, int mods);

      void AddRangeMapping(Input::Key key, int mods, const std::string& name, const float& weight);
      void RemoveRangeMapping(Input::Key key, int mods);

      std::string MapKeyAction(Input::Key key, int mods);
      std::pair<std::string, float> MapKeyRange(Input::Key key, int mods);
    };

  public:
    typedef std::function<void(Input::Action)> ActionCallback;
    typedef std::function<void(Input::Action, float)> RangeCallback;

  private:
    struct MouseInformation {
      double posX;
      double posY;
      double scrollX;
      double scrollY;
      bool isHidden;
      bool isEntered;
      bool btn1;
      bool btn2;
      bool btn3;
      bool btn4;
      bool btn5;
      bool btn6;
      bool btn7;
      bool btn8;
    };
    std::shared_ptr<Context> headContext;
    std::map<std::string, ActionCallback> actionMap;
    std::map<std::string, std::pair<RangeCallback, float>> rangeMap;

  public:
    MouseInformation mouseInfo;

  public:
    void EvaluateKey(Input::Key key, Input::Action action, int mods, double val);

    void BindAction(const std::string& name, ActionCallback func);
    void BindRange(const std::string& name, RangeCallback func, float val = 1.0f);

    void AddContext(Context* con);
    void AddContext(std::shared_ptr<Context> con);

    bool IsContextActivate(Context* con);
    bool IsContextActivate(std::shared_ptr<Context> con);

    void RemoveContext(Context* con);
    void RemoveContext(std::shared_ptr<Context> con);

    void ActivateContext(Context* con);
    void ActivateContext(std::shared_ptr<Context> con);

    void DeactivateContext(Context* con);
    void DeactivateContext(std::shared_ptr<Context> con);

    void ResetPriority(Context* con, int priorityLevel);
    void ResetPriority(std::shared_ptr<Context> con, int priorityLevel);

    inline std::pair<double, double> GetMousePosition() const {
      return { mouseInfo.posX, mouseInfo.posY };
    }

    virtual void SetCursorImage(unsigned char* image, unsigned int width, unsigned int height) { };
    virtual void EnableCursor() { }
    virtual void DisableCursor() { };
    virtual Input::Key GetKey(int keyCode) { return Key::KEY_UNKNOWN; };
    virtual int GetMods(int modBits) { return 0; };
    virtual Input::Action GetAction(int actionCode) { return Action::UNKNOWN; };
    virtual std::shared_ptr<Actor> GetCursorHoveringActor() { return nullptr; };
  };

  /* ===================== VARIABLES ==================== */
  extern std::string title;
  extern std::shared_ptr<Renderer> renderer;
  extern std::shared_ptr<Audio> audio;
  extern std::shared_ptr<Input> input;

  extern std::shared_ptr<ActorManager> actorManager;
  extern std::shared_ptr<Gamemode> gamemode;
  extern std::shared_ptr<Timer> timer;

  extern std::vector<std::shared_ptr<Module>> addons;

  /* ====================== FUNCTIONNS =====================*/
  /**
   * @brief Run the game
   */
  void Run();

  /**
   * @brief Stop the game. Call the OnStop method of all modules.
   */
  void Stop();

  /* ===================== Others ==========================*/

  /**
   * @brief Component-based entity
   */
  class Actor : public Tickable
  {
    friend class ActorManager;

  private:

  public:
    entt::entity id;
    Actor() : id(entt::null) {}
    Actor(const entt::entity& id_) : id(id_) {}
    ~Actor() {};
    operator entt::entity() const { return id; }
    virtual void OnImGui() {};

    inline const bool IsValid() const { return id != entt::null; }

    template <typename T, class... Args> T& AddComponent(Args &&...args)
    {
      return Bored::actorManager->AddComponent<T>(id,
        std::forward<Args>(args)...);
    }

    template <class... Comps>
    inline decltype(auto) Get() const { return Bored::actorManager->Get<Comps...>(id); }
  };

} // namespace Bored
