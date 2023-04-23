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

//std::shared_ptr

std::vector<std::shared_ptr<Module>> addons;

/**
 * @brief Run the game
 */
void Run() {
  timer->OnSetup();
  actorManager->OnSetup();
  gamemode->OnSetup();
  renderer->OnSetup();
  input->OnSetup();
  audio->OnSetup();
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
const glm::mat4 &Renderer::GetProjMat() const { return projector->GetMat(); }

const glm::mat4 Transform::GetMat() const {
  //auto ans = glm::mat4(1.0f);
  auto m = glm::mat4(1.0f);
  //model
  m= glm::translate(glm::mat4(1.0f), pos);
  m= glm::rotate(m, glm::radians(glm::length(rotation)),
                    glm::normalize(rotation));
  m= glm::scale(m, scale);
  return m;
}

void Renderer::UseCamera(std::shared_ptr<Actor> cam)
{
  // TODO check cam component exists
  active_cam = cam;
}


void Input::EvaluateKey(KeyInput::Key key, KeyInput::Action action, int mods, double val)
{
    if (!headContext)
    {
        std::cout << "Did not registed any Context." << std::endl;
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

std::shared_ptr<Actor> Input::GetCursorHoveringActor()
{
    return nullptr;
}

void Input::BindAction(const std::string& name, ActionCallback func)
{
    auto it = actionMap.find(name);
    if (it != actionMap.end())
    {
        std::cout << "Overwriting alias for action function" << std::endl;
    };
    actionMap[name] = func;
}

void Input::BindRange(const std::string& name, RangeCallback func, float weight)
{
    auto it = rangeMap.find(name);
    if (it != rangeMap.end())
    {
        std::cout << "WARNING! Overwriting alias for range function" << std::endl;
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

boolean Input::isContextActivate(Context* con)
{
    return isContextActivate(std::shared_ptr<Context>(con));
}

boolean Input::isContextActivate(std::shared_ptr<Context> con)
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

void Context::ResetPriority(int priority)
{
    priority_ = priority;
}

void Context::Activate()
{
    isActive_ = true;
}

void Context::Deactivate()
{
    isActive_ = false;
}

void Context::Debuggin()
{
    //std::cout << isActive_ << "AS ACTIVATION" << std::endl;
    //std::cout << "Printing Action Map" << std::endl;
    //for (const auto& elem : actionMap_) {
    //    std::cout << elem.first.first << " " << elem.first.second << " " << elem.second << std::endl;
    //};

    //std::cout << "Printing Range Map" << std::endl;
    //for (const auto& elem : rangeMap_) {
    //    std::cout << elem.first.first << " " << elem.first.second << " " << elem.second.first << " " << elem.second.second << std::endl;
    //}
}

void Context::AddActionMapping(KeyInput::Key key, int mods, const std::string& name)
{
    auto it = actionMap_.find({ key, mods });

    // Existed
    if (it != actionMap_.end())
        std::cout << "WARNING! Overwriting old key mapping for actions. Please create a new context to ensure no errors" << std::endl;

    actionMap_[{key, mods}] = name;
}

void Context::AddRangeMapping(KeyInput::Key key, int mods, const std::string& name, const float& weight)
{
    auto it = rangeMap_.find({ key, mods });

    // Existed
    if (it != rangeMap_.end())
        std::cout << "WARNING! Overwriting old key mapping for ranges. Please create a new context to ensure no errors" << std::endl;

    rangeMap_[{key, mods}] = { name, weight };
}

void Context::RemoveActionMapping(KeyInput::Key key, int mods)
{
    auto it = actionMap_.find({ key, mods });

    // Existed
    if (it != actionMap_.end())
        actionMap_.erase(it);
}

void Context::RemoveRangeMapping(KeyInput::Key key, int mods)
{
    auto it = rangeMap_.find({ key, mods });

    // Existed
    if (it != rangeMap_.end())
        rangeMap_.erase(it);
}

std::string Context::MapKeyAction(KeyInput::Key key, int mods)
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

std::pair<std::string, float> Context::MapKeyRange(KeyInput::Key key, int mods)
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
