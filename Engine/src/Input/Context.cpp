#include "Context.h"

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
    std::cout << isActive_ << "AS ACTIVATION" << std::endl;
    std::cout << "Printing Action Map" << std::endl;
    for (const auto& elem : actionMap_) {
        std::cout << elem.first.first << " " << elem.first.second << " " << elem.second << std::endl;
    };

    std::cout << "Printing Range Map" << std::endl;
    for (const auto& elem : rangeMap_) {
        std::cout << elem.first.first << " " << elem.first.second << " " << elem.second.first << " " << elem.second.second << std::endl;
    }
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

    rangeMap_[{key, mods}] = { name, weight};
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
        return next_->MapKeyAction(key, mods);
    }
    auto it = actionMap_.find({ key, mods });
    if (it != actionMap_.end())
        return it->second;
    if (next_)
        return next_->MapKeyAction(key, mods);
    return "";
}

std::pair<std::string,float> Context::MapKeyRange(KeyInput::Key key, int mods)
{
    if (!isActive_) {
        return next_->MapKeyRange(key, mods);
    }
    auto it = rangeMap_.find({ key, mods });
    if (it != rangeMap_.end())
        return it->second;
    if (next_)
        return next_->MapKeyRange(key, mods);
    return {"", 0.0f};
}