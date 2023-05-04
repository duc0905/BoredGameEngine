#include "pch.h"
#include "Core.h"

namespace Bored {
  std::string title = "Bored game";
  std::shared_ptr<Renderer> renderer;
  std::shared_ptr<Audio> audio;
  std::shared_ptr<Input> input;

  std::shared_ptr<ActorManager> actorManager = std::make_shared<ActorManager>();
  std::shared_ptr<Gamemode> gamemode;
  std::shared_ptr<Timer> timer = std::make_shared<Timer>();
  std::unique_ptr<Logger> logger = std::make_unique<InstantLogger>(
    std::make_unique<Logger::ConsoleSource>(), std::make_unique<Logger::StandardFormatter>());

  std::vector<std::shared_ptr<Module>> addons;

  /**
   * @brief Run the game
   */
  void Run() {
    timer->OnSetup();
    // Setup containers
    actorManager->OnSetup();
    // Actors are created here
    gamemode->OnSetup();
    // Populate audio materials
    audio->OnSetup();
    // Populate render materials
    renderer->OnSetup();
    // Setup 
    input->OnSetup();

    for (auto mod : addons)
      mod->OnSetup();

    bool should_close = false;
    while (!should_close) {
      const double dt = timer->GetDt();

      // Poll input
      should_close = input->OnTick(dt) || should_close;

      // Logic - backend

      should_close = timer->OnTick(dt) || should_close;
      should_close = actorManager->OnTick(dt) || should_close;
      should_close = gamemode->OnTick(dt) || should_close;
      for (auto mod : addons)
        should_close = mod->OnTick(dt) || should_close;

      // Output - frontend
      should_close = renderer->OnTick(dt) || should_close;
      should_close = audio->OnTick(dt) || should_close;
    }
  }

  /**
   * @brief Stop the game. Call the OnStop method of all modules.
   */
  void Stop() {
    for (auto mod : addons)
      mod->OnStop();
    renderer->OnStop();
    input->OnStop();
    audio->OnStop();
    gamemode->OnStop();
    actorManager->OnStop();
    timer->OnStop();
  }

  void ActorManager::Delete(entt::entity id) { actor_registry.destroy(id); }
  bool ActorManager::OnTick(double dt) {
    bool shouldClose = false;
    auto view = actor_registry.view<IDToPtr>();
    for (auto id : view) {
      std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
      if (a->OnTick(dt))
        return true;
    }
    return false;
  }
  void ActorManager::OnImGui() {
    auto view = actor_registry.view<IDToPtr>();
    for (auto id : view) {
      std::shared_ptr<Actor> a = view.get<IDToPtr>(id);
      a->OnImGui();
    }
  }
  const glm::mat4& Renderer::GetProjMat() const { return projector->GetMat(); }

  const glm::mat4 Transform::GetMat() const {
    //auto ans = glm::mat4(1.0f);
    auto m = glm::mat4(1.0f);
    //model
    m = glm::translate(glm::mat4(1.0f), pos);
    m = glm::rotate(m, glm::radians(glm::length(rotation)),
      glm::normalize(rotation));
    m = glm::scale(m, scale);
    return m;
  }

  void Renderer::UseCamera(std::shared_ptr<Actor> cam)
  {
    // TODO check cam component exists
    active_cam = cam;
  }


  void Input::EvaluateKey(Input::Key key, Input::Action action, int mods, double val)
  {
    if (!headContext)
    {
      logger->warn("Did not registed any Context.");
      return;
    }
    std::string actionName = headContext->MapKeyAction(key, mods);
    std::pair<std::string, float> rangePack = headContext->MapKeyRange(key, mods);

    if (!(actionName == ""))
    {
      auto it = actionMap.find(actionName);
      if (it != actionMap.end())
        it->second(action);
    }

    if (!(rangePack.first == ""))
    {
      auto it = rangeMap.find(rangePack.first);
      if (it != rangeMap.end())
        it->second.first(action, rangePack.second * val);
    }
  }

  void Input::BindAction(const std::string& name, ActionCallback func)
  {
    auto it = actionMap.find(name);
    if (it != actionMap.end())
    {
      logger->warn("Overwriting alias for action function");
    };
    actionMap[name] = func;
  }

  void Input::BindRange(const std::string& name, RangeCallback func, float weight)
  {
    auto it = rangeMap.find(name);
    if (it != rangeMap.end())
    {
      logger->warn("Overwriting alias for range function");
    };
    rangeMap[name] = { func, weight };
  }

  void Input::AddContext(Context* con)
  {
    AddContext(std::shared_ptr<Context>(con));
  }

  void Input::AddContext(std::shared_ptr<Context> con)
  {
    if (headContext)
    {
      auto cur = headContext;
      while (cur->next_)
      {
        cur = cur->next_;
      }
      cur->next_ = con;
      con->ResetPriority(cur->priority_ + 1);
    }

    else
    {
      headContext = con;
    }

    con->Activate();
  }

  bool Input::IsContextActivate(Context* con)
  {
    return IsContextActivate(std::shared_ptr<Context>(con));
  }

  bool Input::IsContextActivate(std::shared_ptr<Context> con)
  {
    return con->isActive_;
  }

  void Input::RemoveContext(Context* con)
  {
    RemoveContext(std::shared_ptr<Context>(con));
  }

  void Input::RemoveContext(std::shared_ptr<Context> con)
  {
    auto cur = headContext;
    if (cur)
    {
      while (cur->next_)
      {
        if (cur->next_ == con)
        {
          cur->next_ = cur->next_->next_;
          break;
        }
        cur = cur->next_;
      }
      if (headContext == con)
        headContext.reset();
    }
  }

  void Input::ActivateContext(Context* con)
  {
    ActivateContext(std::shared_ptr<Context>(con));
  }

  void Input::ActivateContext(std::shared_ptr<Context> con)
  {
    con->Activate();
  }

  void Input::DeactivateContext(Context* con)
  {
    DeactivateContext(std::shared_ptr<Context>(con));
  }

  void Input::DeactivateContext(std::shared_ptr<Context> con)
  {
    con->Deactivate();
  }
  void Input::ResetPriority(Context* con, int priorityLevel)
  {
    ResetPriority(std::shared_ptr<Context>(con), priorityLevel);
  }

  void Input::ResetPriority(std::shared_ptr<Context> con, int priorityLevel)
  {
    auto cur = headContext;
    std::shared_ptr<Context> target;
    std::shared_ptr<Context> nextLargerContext;
    if (cur)
    {
      while (cur->next_ && (!target && !nextLargerContext))
      {
        if (cur->priority_ <= priorityLevel)
        {
          nextLargerContext = cur;
        }
        if (cur->next_ == con)
        {
          target = cur->next_;
          cur->next_ = cur->next_->next_;
        }
        cur = cur->next_;
      }
      if (target)
      {
        target->next_ = nextLargerContext->next_;
        nextLargerContext->next_ = target;
      }
    }
  }

  std::shared_ptr<Actor> Input::GetCursorHoveringActor()
  {
    return renderer->GetActorAt(mouseInfo.posX, mouseInfo.posY);
  }

  void Input::Context::ResetPriority(int priority)
  {
    priority_ = priority;
  }

  void Input::Context::Activate()
  {
    isActive_ = true;
  }

  void Input::Context::Deactivate()
  {
    isActive_ = false;
  }

  void Input::Context::AddActionMapping(Input::Key key, int mods, const std::string& name)
  {
    auto it = actionMap_.find({ key, mods });

    // Existed
    if (it != actionMap_.end())
      logger->warn("Overwriting old key mapping for actions. Please create a new context to ensure no errors");

    actionMap_[{key, mods}] = name;
  }

  void Input::Context::AddRangeMapping(Input::Key key, int mods, const std::string& name, const float& weight)
  {
    auto it = rangeMap_.find({ key, mods });

    // Existed
    if (it != rangeMap_.end())
      logger->warn("Overwriting old key mapping for ranges. Please create a new context to ensure no errors");

    rangeMap_[{key, mods}] = { name, weight };
  }

  void Input::Context::RemoveActionMapping(Input::Key key, int mods)
  {
    auto it = actionMap_.find({ key, mods });

    // Existed
    if (it != actionMap_.end())
      actionMap_.erase(it);
  }

  void Input::Context::RemoveRangeMapping(Input::Key key, int mods)
  {
    auto it = rangeMap_.find({ key, mods });

    // Existed
    if (it != rangeMap_.end())
      rangeMap_.erase(it);
  }

  std::string Input::Context::MapKeyAction(Input::Key key, int mods)
  {
    if (!isActive_) {
      if (next_)
        return next_->MapKeyAction(key, mods);
      return "";
    }
    auto it = actionMap_.find({ key, mods });
    if (it != actionMap_.end())
      return it->second;
    if (next_)
      return next_->MapKeyAction(key, mods);
    return "";
  }

  std::pair<std::string, float> Input::Context::MapKeyRange(Input::Key key, int mods)
  {
    if (!isActive_) {
      if (next_)
        return next_->MapKeyRange(key, mods);
      return { "", 0.0f };
    }
    auto it = rangeMap_.find({ key, mods });
    if (it != rangeMap_.end())
      return it->second;
    if (next_)
      return next_->MapKeyRange(key, mods);
    return { "", 0.0f };
  }

} // namespace Bored
