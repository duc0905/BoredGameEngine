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

void Context::AddActionMapping(KeyInput::Key key, KeyInput::Mod mods, const std::string& name)
{
    auto it = actionMap_.find({ key, mods });

    // Existed
    if (it != actionMap_.end())
        std::cout << "WARNING! Overwriting old key mapping for actions. Please create a new context to ensure no errors" << std::endl;

    actionMap_[{key, mods}] = name;
}

void Context::AddRangeMapping(KeyInput::Key key, KeyInput::Mod mods, const std::string& name, const float& weight)
{
    auto it = rangeMap_.find({ key, mods });

    // Existed
    if (it != rangeMap_.end())
        std::cout << "WARNING! Overwriting old key mapping for ranges. Please create a new context to ensure no errors" << std::endl;

    rangeMap_[{key, mods}] = { name, weight};
}

void Context::RemoveActionMapping(KeyInput::Key key, KeyInput::Mod mods)
{
    auto it = actionMap_.find({ key, mods });

    // Existed
    if (it != actionMap_.end())
        actionMap_.erase(it);
}

void Context::RemoveRangeMapping(KeyInput::Key key, KeyInput::Mod mods)
{
    auto it = rangeMap_.find({ key, mods });

    // Existed
    if (it != rangeMap_.end())
        rangeMap_.erase(it);
}

std::pair<ActionEnum, std::string> Context::MapKey(KeyInput::Key key, KeyInput::Mod mods, KeyInput::Action)
{
    return std::pair<ActionEnum, std::string>();
}
