#include "Input.hpp"

namespace Bored
{
namespace Frontend
{
void Input::EvaluateKey(Key key, Action action, int mods, double val)
{
    if (!headContext)
    {
        // logger->warn("Did not registed any Context.");
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

void Input::OnSetup()
{
    // Maybe setup the headContext
}

bool Input::OnUpdate(double dt)
{
    window->PollEvents();
    return true;
}

void Input::OnShutdown()
{
    // Clean up contexts
}

Input::Input(Window* w) : window(w)
{
}

Input::~Input()
{
}

void Input::BindAction(const std::string& name, ActionCallback func)
{
    auto it = actionMap.find(name);
    if (it != actionMap.end())
    {
        // logger->warn("Overwriting alias for action function");
    };
    actionMap[name] = func;
}

void Input::BindRange(const std::string& name, RangeCallback func, float weight)
{
    auto it = rangeMap.find(name);
    if (it != rangeMap.end())
    {
        // logger->warn("Overwriting alias for range function");
    };
    rangeMap[name] = {func, weight};
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

// std::shared_ptr<Actor> Input::GetCursorHoveringActor()
// {
//     // return renderer->GetActorAt(mouseInfo.posX, mouseInfo.posY);
// }

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

void Input::Context::AddActionMapping(Key key, int mods, const std::string& name)
{
    auto it = actionMap_.find({key, mods});

    // Existed
    if (it != actionMap_.end()) {}
        // logger->warn("Overwriting old key mapping for actions. Please create a new context to ensure no errors");

    actionMap_[{key, mods}] = name;
}

void Input::Context::AddRangeMapping(Key key, int mods, const std::string& name, const float& weight)
{
    auto it = rangeMap_.find({key, mods});

    // Existed
    if (it != rangeMap_.end()) {}
        // logger->warn("Overwriting old key mapping for ranges. Please create a new context to ensure no errors");

    rangeMap_[{key, mods}] = {name, weight};
}

void Input::Context::RemoveActionMapping(Key key, int mods)
{
    auto it = actionMap_.find({key, mods});

    // Existed
    if (it != actionMap_.end())
        actionMap_.erase(it);
}

void Input::Context::RemoveRangeMapping(Key key, int mods)
{
    auto it = rangeMap_.find({key, mods});

    // Existed
    if (it != rangeMap_.end())
        rangeMap_.erase(it);
}

std::string Input::Context::MapKeyAction(Key key, int mods)
{
    if (!isActive_)
    {
        if (next_)
            return next_->MapKeyAction(key, mods);
        return "";
    }
    auto it = actionMap_.find({key, mods});
    if (it != actionMap_.end())
        return it->second;
    if (next_)
        return next_->MapKeyAction(key, mods);
    return "";
}

std::pair<std::string, float> Input::Context::MapKeyRange(Key key, int mods)
{
    if (!isActive_)
    {
        if (next_)
            return next_->MapKeyRange(key, mods);
        return {"", 0.0f};
    }
    auto it = rangeMap_.find({key, mods});
    if (it != rangeMap_.end())
        return it->second;
    if (next_)
        return next_->MapKeyRange(key, mods);
    return {"", 0.0f};
}

} // namespace Input

} // namespace Bored